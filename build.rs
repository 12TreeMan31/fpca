fn main() {
    let opencv = pkg_config::Config::new()
        .probe("opencv4")
        .expect("Failed to find OpenCV");

    cxx_build::bridge("src/bindings/edgedetect.rs")
        .files(["src/edgedetect.cpp", "src/channels.c"])
        .flag_if_supported("-std=c++14")
        .includes(&opencv.include_paths)
        .include("includes")
        .compile("libopencv_bridge");

    for lib in &opencv.libs {
        println!("cargo:rustc-link-lib={}", lib);
    }

    for path in &opencv.link_paths {
        println!("cargo:rustc-link-search=native={}", path.display());
    }
    /*cc::Build::new()
        .include("src/includes")
        .file("src/bitmap.c")
        .compile("simple");
    println!("cargo::rerun-if-changed=src/bitmap.c");*/
}
