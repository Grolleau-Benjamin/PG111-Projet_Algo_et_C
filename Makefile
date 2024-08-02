CC=gcc
CFLAGS=-Wall -I.
LDFLAGS=-L./lib

.PHONY: all clean doc lib bin run

all: lib bin main mtf_lib huffman_lib bionio_lib lz_lib

lib:
	mkdir -p lib
bin:
	mkdir -p bin

mtf_lib: 
	$(MAKE) -C algo/mtf
huffman_lib:
	$(MAKE) -C algo/huffman
bionio_lib:
	$(MAKE) -C algo/binio
lz_lib:
	$(MAKE) -C algo/LZ

main: main.c mtf_lib huffman_lib bionio_lib lz_lib
	$(CC) $(CFLAGS) $(LDFLAGS) main.c -o bin/main -lmtf -lhuffman -lbinio -llz

run: main
	LD_LIBRARY_PATH=$(pwd)/lib bin/main

clean:
	rm -rf bin/* lib/*.so
	$(MAKE) -C algo/mtf clean
	$(MAKE) -C algo/huffman clean
	$(MAKE) -C algo/binio clean
	$(MAKE) -C algo/LZ clean

test:
	@echo "Running tests..."
	@echo "\n\n\n"
	@echo "Running MTF tests..."
	$(MAKE) -C algo/mtf test
	@echo "\n\n\n"
	@echo "Running Huffman tests..."
	$(MAKE) -C algo/huffman test
	@echo "\n\n\n"
	@echo "Running Binio tests..."
	$(MAKE) -C algo/binio test
	@echo "\n\n\n"
	@echo "Running LZ tests..."
	$(MAKE) -C algo/LZ test

doc:
	@echo "Generating documentation... Please wait, this may take a while."
	@doxygen > /dev/null 2>&1
	@echo "Documentation generated in ./html"
	@echo "Open => $(PWD)/html/index.html"
