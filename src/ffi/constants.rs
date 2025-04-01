/// A simple rexport of the constants found in `constants.h`
/// so that rust and c both agree on the values no matter what
use crate::ffi::bindings;

pub const WIDTH: usize = bindings::WIDTH;
pub const HEIGHT: usize = bindings::HEIGHT;
pub const SIZE: usize = bindings::SIZE;
pub const BACKGROUND: u8 = bindings::BACKGROUND;
pub const FOREGROUND: u8 = bindings::FOREGROUND;
