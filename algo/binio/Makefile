CC=gcc
CFLAGS=-Wall -I./include -fPIC
LDFLAGS=

ifdef DEBUG
	CFLAGS+=-DDEBUG
endif

.PHONY: all clean binio debug

all: ../../lib/libbinio.so

obj:
	mkdir -p obj
bin:
	mkdir -p bin

../../lib/libbinio.so: obj/binio.o
	$(CC) -shared -o $@ $^ $(LDFLAGS)

obj/binio.o: src/binio.c include/binio.h | obj
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f obj/*.o ../../lib/libbinio.so bin/*

binio: main.c obj/binio.o | bin
	$(CC) $(CFLAGS) $(LDFLAGS) -o bin/$@ $^

debug:
	$(MAKE) clean
	$(MAKE) DEBUG=1 binio

test: tests/test_binio.c obj/binio.o | bin
	$(CC) $(CFLAGS) $(LDFLAGS) -o bin/$@ $^
	./bin/test
	$(MAKE) binio
	valgrind --leak-check=full bin/binio
