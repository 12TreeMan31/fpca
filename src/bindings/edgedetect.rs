#[cxx::bridge]
mod edgedetect {
    unsafe extern "C++" {
        include!("simple-detection/includes/cpp/edgedetect.hpp");
        pub fn edgedetect() -> i32;
    }
}
