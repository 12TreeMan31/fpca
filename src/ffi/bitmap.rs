use crate::ffi::bindings::{self, BITMAPFILEHEADER, BITMAPINFOHEADER, RGBQUAD, bmp_full_header};

pub struct BMPHeader {
    pub inner: *mut bmp_full_header,
}
