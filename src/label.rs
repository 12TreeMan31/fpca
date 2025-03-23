use crate::ffi::node::NodeHandle;

const BACKGROUND: u8 = 0;
const FOREGROUND: u8 = 1;

struct Roots {
    n: Vec<(&NodeHandle, Vec<u8>)>,
}

impl Roots {
    fn new() -> Self {
        Self { n: Vec::new() }
    }
    fn push(&mut self, value: &NodeHandle) {
        let mut lab_vec = Vec::new();
        lab_vec.push(value.label());
        self.n.push((value, lab_vec));
    }
    fn remove(&mut self, index: usize) -> (&NodeHandle, Vec<u8>) {
        self.n.remove(index)
    }
    // Takes any two children and merges them together assuming both parents are regestered as roots
    fn union(&mut self, a: &mut NodeHandle, b: &mut NodeHandle) {
        let root = match a.union(b) {
            Some(x) => x,
            None => return,
        };

        let ra = a.find_root();
        let rb = b.find_root();

        self.n.iter().position(|(x, _)| &x == ra);
        self.n.iter().position(|(x, _)| &x == rb);
    }
}

fn ccl_uf(px: &Vec<u8>, width: u32, height: u32) {
    let size = (width * height) as usize;
    // This is to just convay intent
    let dy = [-1, -1, -1, 0]; // This is wrong: should be width
    let dx = [-1, 0, 1, -1];

    // Create a node for each pixel
    let mut labels: Vec<Option<NodeHandle>> = vec![None; size];
    let mut roots: Roots = Roots::new();

    for (i, &pixel) in px.iter().enumerate() {
        if pixel == BACKGROUND {
            continue;
        }
        labels[i] = Some(NodeHandle::new(i));
        let found_labels: Vec<&NodeHandle> = Vec::new();

        // Checks neighboring pixels
        for edge_idx in dx.iter().zip(dy).map(|(&x, y)| i + (x + y) as usize) {
            // This is just the low handing fruit;
            // We need to handle when pixel is on edge of image
            if (edge_idx == usize::MAX) || (edge_idx > size) {
                continue;
            }

            match labels[edge_idx] {
                // Label at that position
                Some(x) => found_labels.push(&x),
                None => continue,
            }
        }

        // Now union all labels together
        if found_labels.len() == 0 {
            // Found a new tree
        }

        found_labels.iter().fold(labels[i], |(acc, x)| {})
    }
}
