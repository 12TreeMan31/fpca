SRC=src
INC=$(SRC)/includes
GEN=$(SRC)/ffi/generated
RES=results

ffi: 
	bindgen src/ffi/bindgen.h -o src/ffi/bindings.rs --raw-line "#![allow(warnings, unused)]"

build:
	cargo build --release

clean: 
	cargo clean
	rm -r $(RES)