use crate::ffi::channels::RingBuffer;
use std::thread;

#[cxx::bridge]
mod edgedetect {
    unsafe extern "C++" {
        include!("simple-detection/includes/edgedetect.hpp");
        // https://docs.rs/cxx/latest/cxx/trait.ExternType.html
        type ring_buffer_t = crate::ffi::bindings::ring_buffer_t;
        pub unsafe fn edges(buffer: *mut ring_buffer_t) -> i32;
    }
}

pub fn edge_finder(buf: &RingBuffer) {
    thread::spawn(|| unsafe { edgedetect::edges(*buf.get_mut()) });
}
