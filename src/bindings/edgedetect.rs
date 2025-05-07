#[cxx::bridge]
pub mod edgedetect {
    unsafe extern "C++" {
        include!("simple-detection/includes/cpp/edgedetect.hpp");
        pub fn edges() -> i32;
    }
}
