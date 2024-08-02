CC=gcc
CFLAGS=-Wall -I./include -fPIC
LDFLAGS=

ifdef DEBUG
	CFLAGS+=-DDEBUG
endif

.PHONY: all clean lz debug
all: obj bin ../../lib/liblz.so

../../lib/liblz.so: obj/lz.o
	$(CC) -shared -o $@ $^ $(LDFLAGS)

obj:
	mkdir -p obj
bin:
	mkdir -p bin

obj/lz.o: src/lz.c include/lz.h | obj
	$(CC) $(CFLAGS) -c -o $@ $<

clean: 
	rm -f obj/*.o ../../lib/liblz.so bin/*

lz: main.c obj/lz.o | bin
	$(CC) $(CFLAGS) $(LDFLAGS) -o bin/$@ $^

debug:
	$(MAKE) clean
	$(MAKE) DEBUG=1 lz

test: tests/test_lz.c obj/lz.o | bin
	$(CC) $(CFLAGS) $(LDFLAGS) -o bin/$@ $^ 
	./bin/test
	$(MAKE) lz
	valgrind --leak-check=full bin/lz
