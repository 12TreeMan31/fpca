fn main() {
    let bindings = bindgen::builder()
        .header("src/ffi/bindgen.h")
        .clang_arg("-I./includes/")
        .generate()
        .expect("Failed to create bindings");

    bindings
        .write_to_file("src/ffi/bindings.rs")
        .expect("Could not write bindings file");
    println!("Generated ffi bindings");

    let opencv = pkg_config::Config::new()
        .probe("opencv4")
        .expect("Failed to find OpenCV");

    cxx_build::bridge("src/ffi/edgedetect.rs")
        .files(["src/edgedetect.cpp", "src/channels.c"])
        .flag_if_supported("-std=c++14")
        .includes(&opencv.include_paths)
        .include("includes/")
        .compile("opencv_bridge");

    for lib in &opencv.libs {
        println!("cargo:rustc-link-lib={}", lib);
    }

    for path in &opencv.link_paths {
        println!("cargo:rustc-link-search=native={}", path.display());
    }

    println!("cargo::rerun-if-changed=src/channels.c");
    println!("cargo::rerun-if-changed=src/edgedetect.cpp");
}
