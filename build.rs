fn main() {
    cc::Build::new()
        .include("src/includes")
        .file("src/bitmap.c")
        .compile("simple");
    println!("cargo::rerun-if-changed=src/bitmap.c");
}
