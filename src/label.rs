use crate::ffi::node::NodeHandle;
use std::ops::{Deref, DerefMut};

const BACKGROUND: u8 = 0;
const FOREGROUND: u8 = 1;

// const WIDTH: usize = 954;
// const HEIGHT: usize = 798;
const WIDTH: usize = 4;
const HEIGHT: usize = 4;
const SIZE: usize = WIDTH * HEIGHT;

struct Roots<'a> {
    inner: Vec<(&'a NodeHandle, Vec<u32>)>,
}

impl<'a> Deref for Roots<'a> {
    type Target = Vec<(&'a NodeHandle, Vec<u32>)>;

    fn deref(&self) -> &Self::Target {
        &self.inner
    }
}
impl<'a> DerefMut for Roots<'a> {
    fn deref_mut(&mut self) -> &mut Self::Target {
        &mut self.inner
    }
}

impl<'a> Roots<'a> {
    fn new() -> Self {
        Self { inner: Vec::new() }
    }

    // Looks for node in roots and returns its index or none if it cannot be found
    fn find_node(&self, node: &NodeHandle) -> Option<usize> {
        self.inner.iter().position(|&(x, _)| *x == *node)
    }
    fn push(&mut self, value: &'a NodeHandle) {
        let mut first_label: Vec<u32> = Vec::new();
        first_label.push(value.label());
        self.inner.push((value, first_label));
    }

    /*
     * takes two children and unions them together, reflecting
     * the new root in this array, returns None if already part of same tree
     * panics if nodes parents are not apart of roots
     */
    fn union(&mut self, a: &mut NodeHandle, b: &mut NodeHandle) -> Option<usize> {
        let root_idx = a.union(b)? as usize;

        // Converts them into a format that we can index into with xor
        let roots = [a, b].map(|x| {
            let x = x.find_root();
            self.find_node(&x)
                .expect("doing union on a node not in roots")
        });

        let idx_a = roots[root_idx];
        // Root to be removed; give its labels vec to the new tree root
        let idx_b = roots[root_idx ^ 1];

        if idx_a < idx_b {
            let (left, right) = self.split_at_mut(idx_b);
            let new_root = &mut left[idx_a];
            let old_root = &mut right[0]; // because idx_b is the first element in the 'right' slice
            new_root.1.extend_from_slice(&old_root.1);
        } else {
            let (left, right) = self.split_at_mut(idx_a);
            let old_root = &mut left[idx_b];
            let new_root = &mut right[0]; // because idx_a is the first element in the 'right' slice
            new_root.1.extend_from_slice(&old_root.1);
        }

        self.remove(idx_b);
        Some(root_idx)
    }
}

fn idx_to_xy(index: usize) -> (usize, usize) {
    let x = index % WIDTH;
    let y = index / WIDTH;
    (x, y)
}

fn get_edges(index: usize, pixels: &[u8]) -> [Option<usize>; 4] {
    // Component vectors
    const DY: [i32; 4] = [-1, -1, -1, 0];
    const DX: [i32; 4] = [-1, 0, 1, -1];

    let mut surroundings: [Option<usize>; 4] = [None; 4];
    let (cur_x, cur_y) = idx_to_xy(index);

    for (k, (&x, &y)) in DX.iter().zip(DY.iter()).enumerate() {
        // Type conversion mess
        let idx = x + (y * WIDTH as i32) + index as i32;
        let idx = idx as usize;

        // Makes sure the index is on the correct line which is helpful on the edges of an image
        let (this_x, this_y) = idx_to_xy(idx);
        let neighbor_chk: bool =
            (this_x as i32 == (cur_x as i32 + x)) && (this_y as i32 == (cur_y as i32 + y));

        // Make this a uszie; we know underflow was originally negative
        // Meant to short circut
        if idx > SIZE || !neighbor_chk {
            continue;
        }

        if pixels[idx] == FOREGROUND {
            surroundings[k] = Some(idx)
        }
    }

    surroundings
}

pub fn ccl_uf(px: &[u8]) -> usize {
    // Every pixel will have a node
    let mut labels: Vec<NodeHandle> = px
        .iter()
        .enumerate()
        .map(|(i, _)| NodeHandle::new(i as u32))
        .collect();
    let mut roots: Roots = Roots::new();

    for (i, &pixel) in px.iter().enumerate() {
        if pixel == BACKGROUND {
            continue;
        }

        let current = &mut labels[i];
        for node_idx in get_edges(i, px) {
            let found = match node_idx {
                Some(x) => &mut labels[x],
                None => continue,
            };
        }

        /*let mut new_node = NodeHandle::new(i as u32);
        // Get each edge and then union them to roots
        for node_idx in get_edges(i, &labels) {
            let found = match node_idx {
                Some(idx) => match &mut labels[idx] {
                    Some(x) => x,
                    None => unreachable!("get_edges should have already properly bounds checked"),
                },
                None => continue,
            };
            roots.union(&mut new_node, found);
        }
        labels[i] = Some(new_node);*/
    }
    roots.len()
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn tetris() {
        let data = [1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1];
        assert_eq!(ccl_uf(&data), 2)
    }
}
