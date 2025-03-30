use crate::ffi::bindings::{self, Node};

#[derive(PartialEq)]
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
    pub fn union(&mut self, other: &mut Self) -> Option<i32> {
        let new_root = unsafe { bindings::node_union(self.inner, other.inner) };
        if new_root == -1 {
            return None;
        }
        return Some(new_root);
    }
    pub fn label(&self) -> u32 {
        unsafe { (*self.inner).label }
    }
}

impl Drop for NodeHandle {
    fn drop(&mut self) {
        unsafe { bindings::node_free(&mut self.inner) };
    }
}

// We don't need this but it makes vec! happy when cloning None
impl Clone for NodeHandle {
    fn clone(&self) -> Self {
        NodeHandle { inner: self.inner }
    }
}
