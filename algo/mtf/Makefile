CC=gcc
CFLAGS=-Wall -I./include -fPIC
LDFLAGS=

ifdef DEBUG
    CFLAGS+=-DDEBUG
endif

.PHONY: all clean mtf debug
all: obj bin ../../lib/libmtf.so

../../lib/libmtf.so: obj/mtf_enc.o obj/mtf_dec.o obj/mtf.o obj/mtf_common.o
	$(CC) -shared -o $@ $^ $(LDFLAGS)

obj:
	mkdir -p obj
bin:
	mkdir -p bin

obj/mtf_enc.o: src/mtf_enc.c include/mtf_enc.h | obj
	$(CC) $(CFLAGS) -c -o $@ $<
obj/mtf_dec.o: src/mtf_dec.c include/mtf_dec.h | obj
	$(CC) $(CFLAGS) -c -o $@ $<
obj/mtf_common.o: src/mtf_common.c include/mtf_common.h | obj
	$(CC) $(CFLAGS) -c -o $@ $<
obj/mtf.o: src/mtf.c include/mtf.h | obj
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f obj/*.o ../../lib/libmtf.so bin/*

mtf: main.c obj/mtf_enc.o obj/mtf_dec.o obj/mtf.o obj/mtf_common.o | bin
	$(CC) $(CFLAGS) $(LDFLAGS) -o bin/$@ $^

debug:
	$(MAKE) clean
	$(MAKE) DEBUG=1 mtf

test: tests/test_mtf.c obj/mtf_enc.o obj/mtf_dec.o obj/mtf.o obj/mtf_common.o | bin
	$(CC) $(CFLAGS) $(LDFLAGS) -o bin/$@ $^
	./bin/test
	$(MAKE) mtf
	valgrind ./bin/mtf
