#include <stdio.h>

int main() {

    char *out_file[] = {"test1.txt", "test2.txt", "test3.txt", "test4.txt", "test5.txt", "test6.txt"};

    FILE *fout;

    fout = fopen(out_file[0], "wt");
    for (int i = 0; i < 1000; i++) {
        fprintf(fout, "a");
    }
    fclose(fout);


    fout = fopen(out_file[1], "wt");
    for (int i = 0; i < 100000; i++) {
        fprintf(fout, "abc");
    }
    fclose(fout);

    fout = fopen(out_file[2], "wt");
    for (int i = 0; i < 10000; i++) {
        fprintf(fout, "abcdefghijklmnopqrstuvwxyz");
    }
    fclose(fout);

    fout = fopen(out_file[3], "wt");
    for (int i = 0; i < 100000; i++) {
        fprintf(fout, "%c", i%256);
    }
    fclose(fout);

    fout = fopen(out_file[4], "wt");
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < i; j++) {
            fprintf(fout, "%c", j%256);
        }
    }
    fclose(fout);

    fout = fopen(out_file[5], "wt");
    for (int i = 50; i >= 0; i--) {
        for (int j = 0; j < i; j++) {
            for (int k = 0; k < j; k++) {
                fprintf(fout, "%c", i);
                fprintf(fout, "%c", i);
                fprintf(fout, "%c", i);
                fprintf(fout, "%c", i);
                fprintf(fout, "%c", i);
                fprintf(fout, "%c", i);
                fprintf(fout, "%c", i);
                fprintf(fout, "%c", i);
                fprintf(fout, "%c", i);
                fprintf(fout, "%c", i);
                fprintf(fout, "%c", i);
                fprintf(fout, "%c", i);
                fprintf(fout, "%c", i);
                
            }
        }
    }
    fclose(fout);
    
    return 0;
}