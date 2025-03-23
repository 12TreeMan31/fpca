use crate::ffi::bindings::{self, Node};

pub struct NodeHandle {
    inner: *mut Node,
    // Make sure to add sub labels in a vector
}

// Need to add a way to free nodes
impl NodeHandle {
    pub fn new(label: u32) -> Self {
        Self {
            inner: unsafe { bindings::node_new(label) },
        }
    }
    pub fn find_root(&self) -> Self {
        Self {
            inner: unsafe { bindings::node_root(self.inner) },
        }
    }
    // Returns None if part of same tree
    pub fn union(&mut self, other: &mut Self) -> Option<Self> {
        let ptr = unsafe { bindings::node_union(self.inner, other.inner) };
        if ptr.is_null() {
            return None;
        }
        return Some(Self { inner: ptr });
    }
    pub fn label(&self) -> u32 {
        unsafe { (*self.inner).label }
    }
}

impl Drop for NodeHandle {
    fn drop(&mut self) {
        unsafe { bindings::node_free(self.inner) };
    }
}

// We don't need this but it makes vec! happy when cloning None
impl Clone for NodeHandle {
    fn clone(&self) -> Self {
        NodeHandle { inner: self.inner }
    }
}
