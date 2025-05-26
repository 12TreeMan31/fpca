fn main() {
    let bindings = bindgen::builder()
        .header("src/bindings/bindgen.h")
        .clang_arg("-I./includes/c")
        .generate()
        .expect("Failed to create bindings");

    bindings
        .write_to_file("src/bindings/bindings.rs")
        .expect("Could not write bindings file");

    let opencv = pkg_config::Config::new()
        .probe("opencv4")
        .expect("Failed to find OpenCV");

    cxx_build::bridge("src/bindings/edgedetect.rs")
        .files(["src/edgedetect.cpp", "src/channels.c"])
        .flag_if_supported("-std=c++14")
        .includes(&opencv.include_paths)
        .include("includes/cpp")
        .include("includes/c")
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
