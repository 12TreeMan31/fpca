use crate::ffi::constants::*;
use crate::node::*;
use std::ops::{Deref, DerefMut};

#[derive(Debug)]
struct Roots {
    /// While it would be nice to set up roots like we did with NodeArena,
    /// we would lose out of a lot on syntax which would make things easy
    inner: Vec<(usize, Vec<usize>)>,
}

impl Deref for Roots {
    type Target = Vec<(usize, Vec<usize>)>;

    fn deref(&self) -> &Self::Target {
        &self.inner
    }
}
impl DerefMut for Roots {
    fn deref_mut(&mut self) -> &mut Self::Target {
        &mut self.inner
    }
}

impl Roots {
    pub fn new() -> Self {
        Self { inner: Vec::new() }
    }

    fn find_node(&self, label: usize) -> Option<usize> {
        self.iter().position(|&(x, _)| x == label)
    }
    pub fn push_node(&mut self, label: usize) {
        let mut first_label: Vec<usize> = Vec::new();
        first_label.push(label);
        self.push((label, first_label));
    }

    pub fn union(&mut self, labels: &mut NodeArena, a: usize, b: usize) -> Option<usize> {
        // Indexs of the 2 roots in self
        let roots = [a, b].map(|x| {
            let label_idx = labels.find_root(x);
            self.find_node(label_idx)
                .expect("looking for a node not regester in roots!")
        });

        let main = labels.union(a, b)?;
        let idx_a = roots[main];
        // Root to be removed; give its labels vec to the new tree root
        let idx_b = roots[main ^ 1];

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
        Some(idx_a)
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
    let mut labels: NodeArena = NodeArena::new();
    let mut roots: Roots = Roots::new();

    for (i, &pixel) in px.iter().enumerate() {
        if pixel == BACKGROUND {
            continue;
        }

        roots.push_node(i);
        for node_idx in get_edges(i, px) {
            // Index of a FORGROUND pixel which we will union to
            let edge = match node_idx {
                Some(x) => x,
                None => continue,
            };
            roots.union(&mut labels, i, edge);
        }
    }

    bounds_boxes(&roots);
    roots.len()
}

struct Labels {
    top_x: Vec<usize>,
    top_y: Vec<usize>,
    bottom_x: Vec<usize>,
    bottom_y: Vec<usize>,
}

impl Labels {
    pub fn new() -> Self {
        Self {
            top_x: Vec::new(),
            top_y: Vec::new(),
            bottom_x: Vec::new(),
            bottom_y: Vec::new(),
        }
    }
}

fn bounds_boxes(roots: &Roots) -> Labels {
    let mut labs: Labels = Labels::new();
    for (_, idxs) in roots.iter() {
        let max = idxs.iter().fold((0, 0), |(acc_x, acc_y), &index| {
            let (mut x, mut y) = idx_to_xy(index);
            if acc_x > x {
                x = acc_x;
            }
            if acc_y > y {
                y = acc_y;
            }
            (x, y)
        });

        let min = idxs
            .iter()
            .fold((usize::MAX, usize::MAX), |(acc_x, acc_y), &index| {
                let (mut x, mut y) = idx_to_xy(index);
                if acc_x < x {
                    x = acc_x;
                }
                if acc_y < y {
                    y = acc_y;
                }
                (x, y)
            });

        labs.top_x.push(max.0);
        labs.top_y.push(max.1);
        labs.bottom_x.push(min.0);
        labs.bottom_y.push(min.1);
    }
    labs
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn label_union() {
        let mut label = NodeArena::new();
        let res = label.union(0, 1).expect("res should not be None!");
        assert_eq!(res, 1);
        let res = label.find_root(0);
        assert_eq!(res, 1);
        let res = label.union(1, 0);
        assert_eq!(res, None);
        let res = label.union(0, 2).expect("res should not be None!");
        assert_eq!(res, 0);
        println!("{:?}", label);
    }

    #[test]
    fn root_union() {
        let data: [u8; SIZE] = [1; SIZE];
        let mut roots = Roots::new();
        let mut labels = NodeArena::new();

        for (i, _) in data.iter().enumerate() {
            roots.push_node(i);
        }

        for (i, _) in data.iter().enumerate() {
            println!("Union on 0 and {i}");
            let result = roots.union(&mut labels, 0, i);
            println!("{i}: {:?}", result);
        }
        assert_eq!(roots.len(), 1);
    }

    // The main edge cases we were running into. Mainly here to varify the logic in `get_edges`
    #[test]
    fn tetris() {
        let data = [1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1];
        assert_eq!(ccl_uf(&data), 2);
        // let res =
    }

    #[test]
    fn out_square() {
        let data = [1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1];
        assert_eq!(ccl_uf(&data), 1)
    }

    #[test]
    fn four_corner() {
        let data = [1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1];
        assert_eq!(ccl_uf(&data), 4)
    }

    #[test]
    fn cross() {
        let data = [1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1];
        assert_eq!(ccl_uf(&data), 1)
    }

    #[test]
    fn diaginal_with_dot() {
        let data = [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1];
        assert_eq!(ccl_uf(&data), 2)
    }
}
