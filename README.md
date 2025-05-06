# FPCA
Code for my Engineering Design and Development class. The goal of this project was to desgin a robot who could collect trash with minimal human input 

## Building
```bash
make ffi
cargo build
```


### Trash-detection

- [Edge detection](https://ietresearch.onlinelibrary.wiley.com/doi/full/10.1049/iet-ipr.2013.0062)
- [Clustering in 3D space](https://stackoverflow.com/questions/3482161/3d-clustering-algorithm)

### Steps needed
- Take Picture
- 

```
├── edge-detection      ; Python OpenCV code used as a proof of concept  
├── src
│   ├── ffi             ; Module used by rust to interface with c/c++ 
│   └── human           ; Code deticated to visualizing each step
├── includes            ; Includes for c/c++
├── test-graphics       ; Generated images created by the `human` module
├── test-images         ; Images to be tested on by `human`
└── tests               ; "Integration test" used for running `human`
```