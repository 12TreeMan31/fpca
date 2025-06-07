use crate::ffi::bindings::{self, ring_buffer_t};
use cxx::{ExternType, type_id};
use std::cell::UnsafeCell;
use std::ops::Deref;
use std::slice;

pub struct RingBuffer {
    inner: UnsafeCell<*mut ring_buffer_t>,
}

impl RingBuffer {
    pub fn new(size: u32) -> Self {
        let inner = unsafe { bindings::init(size) };
        let inner = UnsafeCell::new(inner);

        Self { inner }
    }
    pub fn read(&mut self) -> Option<&[u8]> {
        let res = unsafe { bindings::read(self.inner.get_mut()) };

        if res.is_null() {
            return None;
        }
        Some(unsafe { slice::from_raw_parts(res, 1000) })
    }
}

impl Deref for RingBuffer {
    type Target = UnsafeCell<*mut ring_buffer_t>;

    fn deref(&self) -> &Self::Target {
        &self.inner
    }
}
