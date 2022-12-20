#! /bin/bash

all: LZW

LZW: LZW.o
	g++ LZW.o -o LZW

LZW.o: LZW.cpp
	g++ -c LZW.cpp

compress:
	chmod +x compress_all.sh
	./compress_all.sh

decompress:
	chmod +x decompress_all.sh
	./decompress_all.sh

clean:
	rm -rf *.o LZW

clean_test_folders:
	rm -rf Compressed_files Decompressed_files 

