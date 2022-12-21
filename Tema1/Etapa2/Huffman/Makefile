#! /bin/bash

all: huffman

huffman:compression.o decompression.o hash_table.o heap.o main.o tree.o
	gcc *o -o huffman

compression.o: compression.c
	gcc -c compression.c

decompression.o: decompression.c
	gcc -c decompression.c

hash_table.o: hash_table.c
	gcc -c hash_table.c

heap.o: heap.c
	gcc -c heap.c

main.o: main.c
	gcc -c main.c

tree.o: tree.c
	gcc -c tree.c

compress:
	chmod +x compress_all.sh
	./compress_all.sh

decompress:
	chmod +x decompress_all.sh
	./decompress_all.sh

run:
	chmod +x compress_all.sh
	./compress_all.sh
	chmod +x decompress_all.sh
	./decompress_all.sh

clean:
	rm -rf *.o huffman

clean_test_folders:
	rm -rf Compressed_files Decompressed_files 

