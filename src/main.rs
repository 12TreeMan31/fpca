use std::fs::{self, File};
use std::io::Read;

use jpeg_encoder::{ColorType, Encoder};
use zune_jpeg::zune_core::{colorspace::ColorSpace, options::DecoderOptions};
use zune_jpeg::{ImageInfo, JpegDecoder};

const TEST_DIR: &str = "tests";
const RESULT_DIR: &str = "results";

// See https://www.niwa.nu/2013/05/math-behind-colorspace-conversions-rgb-hsl/
fn rgb_to_hsl(rgb: &[u8; 3]) -> [f32; 3] {
    // First normilize the values [0, 1]
    let rgb: [f32; 3] = rgb.map(|x| x as f32 / 255.0);
    // Min and max rgb
    let min = rgb[0].min(rgb[1].min(rgb[2]));
    let max = rgb[0].max(rgb[1].max(rgb[2]));

    /*
    Now calculate the Luminace value by adding the max and min values and divide by 2.
    */
    let luma = (max + min) / 2.0;

    // If delta == 0 then we know its a shade of gray
    let delta = max - min;
    if delta == 0.0 {
        return [0.0, 0.0, luma];
    };

    /*
    If Luminance is less or equal to 0.5, then Saturation = (max-min)/(max+min)
    If Luminance is bigger then 0.5. then Saturation = ( max-min)/(2.0-max-min)
    */
    let sat = if luma <= 0.5 {
        delta / (max + min)
    } else {
        delta / (2.0 - (max - min))
    };

    /*
    If Red is max, then Hue = (G-B)/(max-min) mod 6
    If Green is max, then Hue = 2.0 + (B-R)/(max-min)
    If Blue is max, then Hue = 4.0 + (R-G)/(max-min)
    */

    let hue = if max == rgb[0] {
        ((rgb[1] - rgb[2]) / delta) % 6.0
    } else if max == rgb[1] {
        2.0 + ((rgb[2] - rgb[0]) / delta)
    } else {
        4.0 + ((rgb[0] - rgb[1]) / delta)
    };
    let hue = hue * 60.0;

    // Gets it to be a value between [0, 1]
    let hue = if hue >= 360.0 {
        hue - 360.0
    } else if hue <= 0.0 {
        hue + 360.0
    } else {
        hue
    };
    let hue = hue / 360.0;
    // let hue = (hue % 360.0) / 360.0;
    // println!("2. {hue}");
    [hue, sat, luma]
}

fn hsl_to_rgb(hsl: &[f32; 3]) -> [u8; 3] {
    // sat 0, shade of gray
    if hsl[1] == 0.0 {
        let val = (hsl[2] * 255.0) as u8;
        return [val, val, val];
    };

    /*
    If Luminance is smaller then 0.5 (50%) then temporary_1 = Luminance x (1.0+Saturation)
    If Luminance is equal or larger then 0.5 (50%) then temporary_1 = Luminance + Saturation – Luminance x Saturation
    */
    let inter_1 = if hsl[2] <= 0.5 {
        hsl[2] * (1.0 + hsl[1])
    } else {
        hsl[2] + hsl[1] - (hsl[2] * hsl[1])
    };
    let inter_2 = 2.0 * hsl[2] - inter_1;

    let t_rgb = [hsl[0] + (1.0 / 3.0), hsl[0], hsl[0] - 0.333];
    let t_rgb = t_rgb.map(|x| x % 1.0);

    /*
    test 1 – If 6 x temporary_R is smaller then 1, Red = temporary_2 + (temporary_1 – temporary_2) x 6 x temporary_R
    test 2 – If 2 x temporary_R is smaller then 1, Red = temporary_1
    test 3 – If 3 x temporary_R is smaller then 2, Red = temporary_2 + (temporary_1 – temporary_2) x (0.666 – temporary_R) x 6
    */
    let mut rgb: [u8; 3] = [0; 3];
    for i in 0..t_rgb.len() {
        let color = if 6.0 * t_rgb[i] < 1.0 {
            inter_2 + (inter_1 - inter_2) * 6.0 * t_rgb[i]
        } else if 2.0 * t_rgb[i] < 1.0 {
            inter_1
        } else {
            inter_2 + (inter_1 - inter_2) * ((2.0 / 3.0) - t_rgb[i]) * 6.0
        };

        rgb[i] = (color * 255.0) as u8;
    }
    rgb
}

fn change_saturation(d: &Vec<u8>) -> Vec<u8> {
    let mut new_image: Vec<u8> = Vec::new();
    for i in (0..d.len()).step_by(3) {
        let rgb: [u8; 3] = [d[i], d[i + 1], d[i + 2]];
        let mut hsl = rgb_to_hsl(&rgb);

        hsl[1] += 1.0;

        let rgb = hsl_to_rgb(&hsl);
        new_image.append(&mut rgb.to_vec());
    }
    new_image
}

fn grayscale(d: &Vec<u8>) -> Vec<u8> {
    let mut new_image: Vec<u8> = Vec::new();
    for i in (0..d.len()).step_by(3) {
        let avg = (d[i] + d[i + 1] + d[i + 2]) / 3;
        new_image.push(avg);
    }
    new_image
}

fn read_image(file_name: &str) -> Result<(Vec<u8>, ImageInfo), Box<dyn std::error::Error>> {
    let file_path = format!("{TEST_DIR}/{file_name}");
    println!("Reading {file_path}");
    // Open
    let mut fd: File = File::open(file_path)?;
    let mut buf: Vec<u8> = Vec::new();
    let _ = fd.read_to_end(&mut buf)?;

    // COLORSPACE!!!!
    let dec_opts = DecoderOptions::default().jpeg_set_out_colorspace(ColorSpace::RGB);

    let mut decoder = JpegDecoder::new_with_options(buf, dec_opts);
    let px_data: Vec<u8> = decoder.decode()?;
    let image_info = decoder.info().unwrap();
    Ok((px_data, image_info))
}

fn write_image(path: &str, data: &Vec<u8>, info: &ImageInfo, color: ColorType) {
    let enc = Encoder::new_file(path, 100).unwrap();
    enc.encode(data, info.width, info.height, color)
        .expect("Could not encode image");
}

fn main() {
    let _ = fs::remove_dir_all(RESULT_DIR);
    fs::create_dir(RESULT_DIR).unwrap();
    let images = fs::read_dir(TEST_DIR).expect("Could not find testing directory");
    for img in images {
        let file_name = match img {
            Ok(img) => {
                let name = img.file_name();
                let name = name.into_string().expect("Invalid File");
                format!("{}", name)
            }
            Err(e) => {
                println!("{e}");
                continue;
            }
        };

        // Gets rid of the file extenstion for nicer looking names
        let new_name = {
            match file_name.split_once('.') {
                Some((n, _)) => n.to_string(),
                None => file_name.clone(),
            }
        };

        // Creates a dir for all of image tests we are doing
        let res_path = format!("{RESULT_DIR}/{new_name}");
        fs::create_dir(&res_path).unwrap();

        let (px_data, image_info) = read_image(&file_name).expect("Failed to read image");
        let sat_data = change_saturation(&px_data);
        let gray_data = grayscale(&px_data);
        let sat_gray_data = grayscale(&sat_data);

        let sat_path = format!("{res_path}/sat.jpg");
        write_image(&sat_path, &sat_data, &image_info, ColorType::Rgb);

        let gray_path = format!("{res_path}/original-graycale.jpg");
        write_image(&gray_path, &gray_data, &image_info, ColorType::Luma);

        let gray_path = format!("{res_path}/sat-graycale.jpg");
        write_image(&gray_path, &sat_gray_data, &image_info, ColorType::Luma);
    }
}
