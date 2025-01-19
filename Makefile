RES='results/'

build:
	cargo build --release

clean: 
	cargo clean
	rm -r $(RES)