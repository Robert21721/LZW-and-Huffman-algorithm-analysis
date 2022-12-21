#! /bin/bash

all: build

build:
	cd ./Huffman && make -f Makefile --no-print-directory
	cd ./LZW && make -f Makefile --no-print-directory

run-p1:
	cd ./Huffman && make run -f Makefile --no-print-directory

run-p2:
	cd ./LZW && make run -f Makefile --no-print-directory

clean:
	cd ./Huffman && make clean -f Makefile --no-print-directory
	cd ./LZW && make clean -f Makefile --no-print-directory
	rm -rf out/Huffman/*
	rm -rf out/LZW/*
