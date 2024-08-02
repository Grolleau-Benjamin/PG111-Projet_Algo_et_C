CC=gcc
CFLAGS=-Wall -I./include -fPIC
LDFLAGS=

ifdef DEBUG
    CFLAGS+=-DDEBUG
endif

.PHONY: all clean huffman debug
all: obj bin ../../lib/libhuffman.so

obj:
	mkdir -p obj
bin:
	mkdir -p bin

../../lib/libhuffman.so: obj/huffman_enc.o obj/huffman_dec.o obj/huffman.o obj/binio.o
	$(CC) -shared -o $@ $^ $(LDFLAGS)

obj/huffman_enc.o: src/huffman_enc.c include/huffman_enc.h | obj
	$(CC) $(CFLAGS) -c -o $@ $<

obj/huffman_dec.o: src/huffman_dec.c include/huffman_enc.h | obj
	$(CC) $(CFLAGS) -c -o $@ $<

obj/huffman.o: src/huffman.c include/huffman.h | obj
	$(CC) $(CFLAGS) -c -o $@ $<

obj/binio.o: ../binio/src/binio.c ../binio/include/binio.h | obj
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f obj/*.o ../../lib/libhuffman.so bin/*

huffman: main.c obj/huffman_enc.o obj/huffman_dec.o obj/huffman.o obj/binio.o | bin
	$(CC) $(CFLAGS) $(LDFLAGS) -o bin/$@ $^

debug:
	$(MAKE) clean
	$(MAKE) DEBUG=1 huffman

test: tests/test_huffman.c obj/huffman_enc.o obj/huffman_dec.o obj/huffman.o obj/binio.o | bin
	$(CC) $(CFLAGS) $(LDFLAGS) -o bin/$@ $^
	./bin/test
	$(MAKE) huffman
	valgrind --leak-check=full bin/huffman
