Pentru rularea tuturor testelor se va da comanda "make" din radacina folderului
Aceasta va executa Makefile ul din directoarele LZW si Huffman si va creea executabilele

Pentru rularea algoritmului Huffman se va de comanda "make run-p1" din folderul radacina
Aceasta va creea in folderul Out/Huffman, cele doua foldere cu Compressed/ Decompressed_files

Pentru rularea algoritmul LZW se va da comanda "make run-p2" din folderul radacina
Aceasta va creea in folderul Out/LZW, cele doua foldere cu Compressed/ Decompressed_files

Comanda "make clean" va sterge atat executabilele si fisierele obiect, cat si continutul folderelor de out

Ambele foldere cu algoritmi contin cate un Makeile propriu si 2 scripturi ce testeaza automat compresia sau
decomprimarea datelor. Aceste fisiere Makefile vin si cu reguli de "make compress" si make "decompress",
ele adaugand in folderul de out fisierele comprimate/ decomplrimate in urma rularii algoritmului.

Pentru implementarea compresiei Huffman s-a folosit un minheap, un arbore Huffman si o tabela de disperise pentru
accesul mai rapid la codurile literelor.

Pentru implementarea algoritmului LZW s-a folosit un dictionar si un vector auxiliar de inturi.