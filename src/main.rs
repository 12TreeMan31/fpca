#![feature(slice_as_chunks)]
#![feature(iter_map_windows)]
#![feature(array_chunks)]
#![feature(iter_array_chunks)]

use crate::ffi::constants::*;
use edge_detection::*;
use image::GrayImage;
use std::fs::{self, File};
use std::io::Read;

mod distance;
mod edge;
mod ffi;
mod human;
mod label;
mod node;

const TEST_DIR: &str = "tests";
const RESULT_DIR: &str = "results";

/*fn read_image(file_name: &str) -> Result<(Vec<u8>, ImageInfo), Box<dyn std::error::Error>> {
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
}*/

fn main() {
    let image_data = todo!();
}
