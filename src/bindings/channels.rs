use crate::bindings::bindings::{self, ring_buffer};
use crate::bindings::constants::*;
use cxx::{ExternType, type_id};
use std::slice;

unsafe impl ExternType for ring_buffer {
    type Id = type_id!("ring_buffer");
    type Kind = cxx::kind::Opaque;
}

pub struct RingBuffer {
    pub inner: *mut ring_buffer,
}

unsafe impl Sync for RingBuffer {}

impl RingBuffer {
    pub fn new(size: u32) -> Self {
        RingBuffer {
            inner: unsafe { bindings::ring_new(size) },
        }
    }
    pub fn read<'a>(self) -> Option<&'a [u8]> {
        unsafe {
            let newdata = bindings::ring_read(self.inner);
            if newdata.is_null() {
                return None;
            }
            Some(slice::from_raw_parts(newdata, SIZE))
        }
    }
}
