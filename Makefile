RES='results/'
SRC='src/'

build:
	cargo build --release

clean: 
	cargo clean
	rm -r $(RES)