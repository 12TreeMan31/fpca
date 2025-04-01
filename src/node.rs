/*
 * Originally we used OOP to handle union-find,
 * something I have noticed though is that we don't
 * care about the individual objects. We care
 * about a nodes relationship to another node
 *
 * My thinking is that instead we use the DOD approach which
 * which should reduce the complexity of the whole project.
 * It will stop writing our program in terms of an individual
 * node, and instead a whole "arena"
 *
 * Also removes big parts of the c ffi interface :P
 */

use crate::ffi::constants::*;

// We will know the hight and width of the image at compile time
/// The index in the array is the label
#[derive(Debug)]
pub struct NodeArena {
    parent: [usize; SIZE],
    rank: [usize; SIZE],
}

impl NodeArena {
    pub fn new() -> Self {
        let mut parent: [usize; SIZE] = [0; SIZE];
        for i in 0..SIZE {
            parent[i] = i
        }

        Self {
            parent,
            rank: [0; SIZE],
        }
    }

    /// Recursively looks for the root node the the tree and returns the index of the root
    pub fn find_root(&self, start: usize) -> usize {
        let parent = self.parent[start];
        if start == parent {
            return start;
        }
        self.find_root(parent)
    }

    /*
     * Merges node left and right setting the node with the highest
     * rank as the new parent. You MUST called node_root on
     * both nodes before calling this function returns 0 if
     * left is the new root, 1 if right
     */
    pub fn union(&mut self, a: usize, b: usize) -> Option<usize> {
        let l_root = self.find_root(a);
        let r_root = self.find_root(b);

        if l_root == r_root {
            return None;
        }

        // Who the new root is
        let new_root = if self.rank[l_root] > self.rank[r_root] {
            self.parent[r_root] = l_root;
            Some(0)
        } else if self.rank[l_root] < self.rank[r_root] {
            self.parent[l_root] = r_root;
            Some(1)
        } else {
            self.parent[l_root] = r_root;
            self.rank[r_root] += 1;
            Some(1)
        };

        new_root
    }
}
