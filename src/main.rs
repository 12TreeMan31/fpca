#![feature(iter_map_windows)]
#![feature(array_chunks)]
#![feature(iter_array_chunks)]

// use crate::ffi::constants::*;
// use image::{self, DynamicImage, GrayImage, ImageBuffer, Luma};
// use std::sync::atomic::AtomicBool;
// use zune_jpeg::zune_core::{colorspace::ColorSpace, options::DecoderOptions};
// use zune_jpeg::{ImageInfo, JpegDecoder};

mod ffi;
mod human;

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

/* STEPS FOR PROCESSING IMAGES
 * Start
 * - Init the camera (using c to make a rust wrapper for ease of use)
 * - Hand the cameras fd to the main rust program
 * - Rust spawn a child process in python that will have the OpenCV code
 *      While its not amazing its the fastest solution givin the amount of time we have left
 *
 * Main
 * - Reseave the camera data and do basic cleanup of image in rust (calvin)
 * - Send to python via pipes to do canny operation
 * - Send edges back to rust to do CCL because opencv does not implmenet 8-way
 *      The reason why it doesn't is because checkboard patterns are a thing BUT
 *      4-way is not sutable for our prototype as it creates to many labels
 * - Take generated labels and convert them into angles reletive to the camera
 * - Next we must figure out how the labels moved compaired to the previous image (look into optical mice)
 * - Send one of the angles to the arduino using the UART connection
 *
 * Arduino
 * - Wait to get a angle from the pi
 * - Once received just move forward (optinal: robot can rotate)
 */

fn main() {}
