use jpeg_encoder::{ColorType, Encoder};
use std::fs::{self, File};
use std::io::Read;
use zune_jpeg::zune_core::{colorspace::ColorSpace, options::DecoderOptions};
use zune_jpeg::{ImageInfo, JpegDecoder};

const TEST_DIR: &str = "test-images";
const GRAPHICS_DIR: &str = "test-graphics";

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

#[test]
fn max_sat() {}

#[test]
fn hue_split_color() {}

#[test]
fn hue_split_contrast() {}

#[test]
fn generate_edges() {}

#[test]
fn blur_edges() {}
