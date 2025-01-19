use crate::sat;

pub fn group(d: &Vec<u8>) -> Vec<Vec<u8>> {
    let mut image_groups: Vec<Vec<u8>> = vec![Vec::new(); 6];

    for i in (0..d.len()).step_by(3) {
        let rgb: [u8; 3] = [d[i], d[i + 1], d[i + 2]];
        let hsl = sat::rgb_to_hsl(&rgb);

        let sector = (hsl[0] * 6.0).floor() as usize;
        for (j, v) in image_groups.iter_mut().enumerate() {
            if j == sector {
                v.append(&mut rgb.to_vec());
            } else {
                v.append(&mut [0, 0, 0].to_vec());
            }
        }
    }

    image_groups
}

pub fn group_contrast(d: &Vec<u8>) -> Vec<Vec<u8>> {
    let mut image_groups: Vec<Vec<u8>> = vec![Vec::new(); 6];

    for i in (0..d.len()).step_by(3) {
        let rgb: [u8; 3] = [d[i], d[i + 1], d[i + 2]];
        let hsl = sat::rgb_to_hsl(&rgb);

        let sector = (hsl[0] * 6.0).floor() as usize;
        for (j, v) in image_groups.iter_mut().enumerate() {
            if j == sector {
                v.append(&mut [255, 255, 255].to_vec());
            } else {
                v.append(&mut [0, 0, 0].to_vec());
            }
        }
    }

    image_groups
}
