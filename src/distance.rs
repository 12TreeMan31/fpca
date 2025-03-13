// Find an object through some score
// FOV of iphone 12 camera
// const fov: i32 = 120;

// Giving 2 points that form a box, return a vec of all pixels in that region
fn get_area<'a>(
    px: &'a Vec<u8>,
    dimensions: (usize, usize),
    region: [(usize, usize); 2],
) -> Option<Vec<&'a [u8]>> {
    // Make sure coordinates lie within the image
    for (x, y) in region {
        if x > dimensions.0 || y > dimensions.1 {
            return None;
        }
    }

    let mut area: Vec<&[u8]> = Vec::new();

    let height = region[1].1 - region[0].1;
    let width = region[1].0 - region[0].0;

    for i in height..0 {
        let cursor = i * dimensions.1 + region[0].0;
        let area_slice = &px[cursor..cursor + width];
        area.push(area_slice);
    }

    Some(area)
}

fn get_angle(point: &[i32; 2], dimensions: &[i32; 2], fov: f32) -> f32 {
    // Distance from center of image on the x
    let dist = point[0] - dimensions[0];
    let radius = (360 * dist.abs()) as f32 / (2.0 * 3.14 * fov);
    let arc_angle = (360 * dist) as f32 / (2.0 * 3.14 * radius);
    arc_angle
}
