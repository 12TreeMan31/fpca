use crate::bindings::channels::RingBuffer;
use std::thread;

#[cxx::bridge]
mod edgedetect {
    unsafe extern "C++" {
        include!("simple-detection/includes/cpp/edgedetect.hpp");
        // https://docs.rs/cxx/latest/cxx/trait.ExternType.html
        type ring_buffer = crate::bindings::bindings::ring_buffer;
        pub unsafe fn edges(buffer: *mut ring_buffer) -> i32;
    }
}

pub fn edge_finder(buf: &RingBuffer) {
    thread::spawn(|| {
        unsafe { edgedetect::edges(buf.inner) };
    });
}
