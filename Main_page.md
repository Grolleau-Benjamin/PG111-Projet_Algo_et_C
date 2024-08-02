# Documentation : 
The documentation for the compression and decompression modules is available in the following links:
- [binio](./binio_8h.html)
- [mtf](./dir_ace1b91e8e4bc42857186f41e9cbc805.html)
    - [mtf_test](./test__mtf_8c.html)
- [huffman](./dir_2b5c7f96499300571d297a423c5f9f16.html)
- [LZ](./dir_1dee4b13f9db3e0ad85f1f5125592bad.html)

The `common` directory contains debugging output as well as a number of error messages.

### Binio
The `binio` module allows for reading and writing binary files. It is used for reading and writing compressed and decompressed files.

### MTF
The `mtf` module implements the Move To Front algorithm. It is used for compressing and decompressing files.

### Huffman
The `huffman` module implements the Huffman algorithm. It is used for compressing and decompressing files.

### LZ
The `LZ`module is actually not finished. It is fonctionnal for a byte per byte compression but not for a bit per bit compression. We have faced a lot of issues with this algorithm and we have decided to give a proper implementation of the byte per byte implementation instead of doing a sloppy bit per bit implementation. This implementation is not as efficient as the bit per bit one for a small file but it is way equivalent for a big file. This is why we have decided to keep this implementation.