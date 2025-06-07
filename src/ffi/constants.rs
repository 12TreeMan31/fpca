/// A simple rexport of the constants found in `constants.h`
/// so that rust and c both agree on the values no matter what
use crate::bindings::bindings;

pub const WIDTH: usize = bindings::WIDTH as usize;
pub const HEIGHT: usize = bindings::HEIGHT as usize;
pub const SIZE: usize = bindings::SIZE as usize;
pub const BACKGROUND: u8 = bindings::BACKGROUND;
pub const FOREGROUND: u8 = bindings::FOREGROUND;
