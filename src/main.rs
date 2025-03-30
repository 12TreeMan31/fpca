#![feature(slice_as_chunks)]
#![feature(iter_map_windows)]
#![feature(array_chunks)]
#![feature(iter_array_chunks)]

use std::fs::{self, File};
use std::io::Read;
use std::thread::{self, JoinHandle};

use jpeg_encoder::{ColorType, Encoder};
use zune_jpeg::zune_core::{colorspace::ColorSpace, options::DecoderOptions};
use zune_jpeg::{ImageInfo, JpegDecoder};

mod distance;
mod ffi;
mod grouping;
mod label;
mod sat;

const TEST_DIR: &str = "tests";
const RESULT_DIR: &str = "results";

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

fn run_test(new_name: String, file_name: String) {
    // Creates a dir for all of current image tests we are doing
    let res_path = format!("{RESULT_DIR}/{new_name}");
    fs::create_dir(&res_path).unwrap();

    let (px_data, image_info) = read_image(&file_name).expect("Failed to read image");
    let sat_data = sat::change_saturation(&px_data);
    let gray_data = grayscale(&px_data);

    let sat_path = format!("{res_path}/sat.jpg");
    write_image(&sat_path, &sat_data, &image_info, ColorType::Rgb);

    let gray_path = format!("{res_path}/graycale.jpg");
    write_image(&gray_path, &gray_data, &image_info, ColorType::Luma);

    let calvin = format!("{res_path}/calvin-test");
    fs::create_dir(&calvin).unwrap();

    let calvin_data = grouping::group(&sat_data);
    for (i, v) in calvin_data.iter().enumerate() {
        let degree = i * 60;
        let hue_path = format!("{calvin}/{degree}.jpg");
        write_image(&hue_path, v, &image_info, ColorType::Rgb);
    }

    let calvin = format!("{res_path}/calvin-WHITE-test");
    fs::create_dir(&calvin).unwrap();

    let calvin_data = grouping::group_contrast(&px_data);
    for (i, v) in calvin_data.iter().enumerate() {
        let degree = i * 60;
        let hue_path = format!("{calvin}/{degree}.jpg");
        write_image(&hue_path, v, &image_info, ColorType::Rgb);
    }
    /*let ne_dat = grouping::divid(&sat_data, image_info.width, image_info.height, 10);
    let ne_path = format!("{res_path}/divid.jpg");
    write_image(&ne_path, &ne_dat, &image_info, ColorType::Rgb);*/

    /*let rgb_data = grouping::split(&px_data);
    for i in 0..3 {
        let path = format!("{res_path}/{i}.jpg");
        write_image(&path, &rgb_data[i], &image_info, ColorType::Rgb);
    }*/

    println!("Finished test {new_name}");
}

/*
    Open Cameras
    Read bitmap
    Find edges
    Find objects
    Send location of object to arduino
*/

fn main() {
    let mut handles: Vec<JoinHandle<()>> = Vec::new();
    label::ccl_uf(&[1]);
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

        let handle = thread::spawn(move || run_test(new_name, file_name));
        handles.push(handle);
    }
    for _ in 0..handles.len() {
        let h = handles.pop().unwrap();
        h.join().unwrap();
    }
}
