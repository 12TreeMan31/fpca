use crate::ffi::constants::*;
use crate::human::sat;

/// Converts the pixel data into N sectors based on the hues on the color
/// wheel which all have the same arc length. Resulting array will be 1/3
/// of SIZE as we are converting rgb to sectors
fn into_sectors<const N: usize>(image: &[u8]) -> [usize; SIZE / 3] {
    // Size of new array will be 1/3 of old because of rgb values
    image
        .array_chunks::<3>()
        .map(|rgb| {
            let hsl = sat::rgb_to_hsl(rgb);
            (hsl[0] * N).floor() as usize
        })
        .collect()
}

pub fn group_contrast<const N: usize>(image: &[u8]) -> [[u8; SIZE]; N] {
    let sectors = into_sectors::<N>(image);

    let mut image_groups: [[u8; SIZE]; N] = [[0; SIZE]; N];
    for (i, &sec) in sectors.iter().enumerate() {
        let i = i * 3;
        image_groups[sec][i] = 255;
        image_groups[sec][i + 1] = 255;
        image_groups[sec][i + 2] = 255;
    }
    image_groups
}

pub fn group_colors<const N: usize>(image: &[u8]) -> [[u8; SIZE]; N] {
    let sectors = into_sectors::<N>(image);

    let mut image_groups: [[u8; SIZE]; N] = [[0; SIZE]; N];
    for (i, (&sec, &rgb)) in sectors.iter().zip(image.array_chunks::<3>()).enumerate() {
        let i = i * 3;
        image_groups[sec][i] = rgb[0];
        image_groups[sec][i + 1] = rgb[1];
        image_groups[sec][i + 2] = rgb[2];
    }
    image_groups
}
/*pub fn group(d: &Vec<u8>) -> Vec<Vec<u8>> {
    let mut image_groups: Vec<Vec<u8>> = vec![Vec::new(); 6];

    for i in (0..d.len()).step_by(3) {
        let rgb: [u8; 3] = [d[i], d[i + 1], d[i + 2]];
        let hsl = sat::rgb_to_hsl(&rgb);

        let sector: usize = (hsl[0] * 6.0).floor() as usize;
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
}*/

/*pub fn divid(d: &Vec<u8>, width: u16, height: u16, n: u16) -> Vec<u8> {
    let w = width / n;
    let h = height / n;

    /*
    int column = index % numberOfColumns;
    int row = (index - column) / numberOfColumns;
    */

    let mut div_chunks: Vec<Vec<&[u8]>> = (0..n * n).map(|_| Vec::new()).collect();

    let mut j = 0;
    for (i, rgb_slice) in d.chunks(3 * n as usize).enumerate() {
        if (i == 0) {
            j += 1;
        }
        div_chunks[(i % n as usize) * j].push(rgb_slice);
    }

    // Averages the colors of a single block
    let avg_colors = |colors: &[u8]| -> [u8; 3] {
        let len = colors.len() / 3;
        let sum = colors.chunks(3).fold([0; 3], |acc: [u8; 3], chunk: &[u8]| {
            [acc[0] + chunk[0], acc[1] + chunk[1], acc[2] + chunk[2]] // Add the new rgb values to acc
        });

        [
            (sum[0] as usize / len) as u8,
            (sum[1] as usize / len) as u8,
            (sum[2] as usize / len) as u8,
        ]
    };

    let f: Vec<u8> = div_chunks
        .iter()
        .flat_map(|x| x.iter().map(|u| avg_colors(u)))
        .flatten()
        .collect();

    for i in 0..d.len() {

    }
}*/

pub fn split(d: &Vec<u8>) -> Vec<Vec<u8>> {
    let mut grouping: Vec<Vec<u8>> = vec![Vec::new(); 3];
    for (i, d) in d.iter().enumerate() {
        for (j, v) in grouping.iter_mut().enumerate() {
            if i % 3 == j {
                v.push(*d);
            } else {
                v.push(0);
            }
        }
    }
    grouping
}
