SRC=src
INC=$(SRC)/includes
GEN=$(SRC)/ffi/generated
TESTS=tests

TEST_IMGS=test-images
GRAPH_DIR=test-graphics

ffi: 
	bindgen src/ffi/bindgen.h -o src/ffi/bindings.rs --raw-line "// [allow(warnings)]"

debug: ffi
	cargo build
	rust-gdb target/debug/simple-detection

build:
	cargo build --release

clean: 
	cargo clean
	rm -r $(RES)

ccc:
	g++ $(SRC)/edgedetect.cpp $(SRC)/channels.c -I $(INC) -Wall `pkg-config --cflags --libs opencv4`

real:
	cross build --target arm-unknown-linux-gnueabihf

graphics:
	bash $(TESTS)/setup.sh