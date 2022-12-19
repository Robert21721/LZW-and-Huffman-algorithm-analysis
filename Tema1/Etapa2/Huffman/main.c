#include "header.h"

int main(int argc, char *argv[]) {

    if (argc < 4 || argc > 5) {
        printf("invalid nr of arguments\n");
        return -1;
    }
    if (strcmp(argv[1], "-c") == 0) {
        compression(argv[2] , argv[3]);
    }

    if (strcmp(argv[1], "-d") == 0) {
        decompression(argv[2], argv[3]);
    }

}