#include "header.h"

void compression(char *file_name_in, char *file_name_out) {

    FILE *fin1 = fopen(file_name_in, "rb");
    FILE *fout = fopen(file_name_out, "wb");
    TNode *r = createTree(fin1, fout);
    fclose(fin1);
    restore_parent_link(r);

    TNode **hTable = init_hashT();
    create_hashTable(hTable, r);

    FILE *fin2 = fopen(file_name_in, "rb");

    char ch;
    unsigned int a = 0;

    int pos = 8 * sizeof(int);
    unsigned int nr_to_write = 0;
    
    while (fread(&ch, sizeof(char), 1, fin2)) {
        unsigned int cod = get_code(hTable[ch]).cod;
        int len = get_code(hTable[ch]).len;

        if (len <= pos) {
            nr_to_write = nr_to_write + (cod << (pos - len));
            pos = pos - len;
        } else {
            int p1, p2;
            p1 = cod >> (len - pos);
            nr_to_write = nr_to_write + p1;
            fwrite(&nr_to_write, sizeof(unsigned int), 1, fout);
            // fflush(fout);
            p2 = cod << (8 * sizeof(int) + pos - len);
            

            pos = 8 * sizeof(int) + pos - len;
            nr_to_write = p2;
        }
    }

    fwrite(&nr_to_write, sizeof(unsigned int), 1, fout);
    // fflush(fout);

    fclose(fin2);
    fclose(fout);
}
