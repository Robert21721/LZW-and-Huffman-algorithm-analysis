#include "function_headers.h"
#define X 50
#define ONE 1
#define TWO 2
#define THREE 3
#define FOUR 4
#define FIVE 5
#define SIX 6
#define SEVEN 7

void parser() {
    char command[X] = "", backup_command[X] = "", path[X] = "";

    // fh - FilehHeader     ih - InfoHeader
    bmp_fileheader *fh = (bmp_fileheader*) malloc( sizeof(bmp_fileheader));
    bmp_infoheader *ih = (bmp_infoheader*) malloc(sizeof(bmp_infoheader));

    FILE *f_in = NULL, *f_in2 = NULL, *f_out = NULL;
    bmp_bitmap **mat = NULL, **mat2 = NULL;

    bmp_bitmap color;
    color.Blue = 0;
    color.Green = 0;
    color.Red = 0;

    int line_width = 1;

    while (1) {
        // citesc cmoanda, streg \n de la final si fac un backup
        fgets(command, sizeof(command), stdin);

        // strcpy(command + strlen(command) - 1, command + strlen(command));
        command[strlen(command) - 1] = '\0';

        // strcpy(backup_command, command);
        snprintf(backup_command, strlen(command) + 1, "%s", command);

        char arg[X][X] = {""}, *p = NULL, sep[] = " ";
        int n = 0;

        // retin fiecare cuvant din comanda in vectorul arg[]
        p = strtok(backup_command, sep);

        while (p) {
            // strcpy(arg[n++], p);
            snprintf(arg[n++], strlen(p) + 1, "%s", p);
            p = strtok(NULL, sep);
        }

        if (strstr(command, "edit")) {
        // daca a fost gasit in comanda cuvantul "edit"
        // retin calea de unde va fi deschis fisierul in path
        // deschid fisierul, apelez functia de edit
        // initializez matricea de pixeli mat

            // strcpy(path, arg[ONE]);
            snprintf(path, strlen(arg[ONE]) + 1, "%s", arg[ONE]);

            f_in = fopen(path, "rb");

            edit(f_in, path, fh, ih);
            mat = matrix(f_in, ih);
        }

        if (strstr(command, "save")) {
        // daca a fost gasit in comanda cuvantul "edit"
        // retin calea unde va fi salvat fisierul in path
        // deschid fisierul, apelez functia save

            // strcpy(path, arg[ONE]);
            snprintf(path, strlen(arg[ONE]) + 1, "%s", arg[ONE]);

            f_out = fopen(path, "wb");
            save(f_out, path, fh, ih, mat);
        }

        if (strstr(command, "insert")) {
        // daca a fost gasit in comanda cuvantul "insert"
        // aloc dinamic vectori infoheader2 si fileheader2 pentru noua matrice
        // retin in path calea de inde va fi deschis fisierul
        // retin in x si y coordonatele la care va fi inserata noua imagine
        // apelez edit pentu noua matrice si completez matricea de biti
        // apelez insert
            unsigned int x = 0, y = 0;

            bmp_fileheader *fh2 = (bmp_fileheader*) malloc(sizeof(bmp_fileheader));
            bmp_infoheader *ih2 = (bmp_infoheader*) malloc(sizeof(bmp_infoheader));

            // strcpy(path, arg[ONE]);
            snprintf(path, strlen(arg[ONE]) + 1, "%s", arg[ONE]);

            y = atoi(arg[TWO]);
            x = atoi(arg[THREE]);

            f_in2 = fopen(path, "rb");
            edit(f_in2, path, fh2, ih2);
            mat2 = matrix(f_in2, ih2);

            insert(ih2, fh2, ih, mat, mat2, y, x);
        }

        if (strstr(command, "set draw_color")) {
        // setez noua culoare
            color.Red = atoi(arg[TWO]);
            color.Green = atoi(arg[THREE]);
            color.Blue = atoi(arg[FOUR]);
        }

        if (strstr(command, "set line_width")) {
        // setez grosimea liniei
            line_width = atoi(arg[TWO]);
        }

        if (strstr(command, "draw line")) {
        // retin coordonatele punctelor si desenez linia
            int x1 = 0, x2 = 0, y1 = 0, y2 = 0;

            y1 = atoi(arg[TWO]);
            x1 = atoi(arg[THREE]);
            y2 = atoi(arg[FOUR]);
            x2 = atoi(arg[FIVE]);

            draw_line(y1, x1, y2, x2, mat, color, line_width, ih);
        }

        if (strstr(command, "draw rectangle")) {
        // retin coordonatele punctului de inceput, lungimea si latimea
        // desenez dreptunghiul
            int x1 = 0, y1 = 0, width = 0, height = 0;

            y1 = atoi(arg[TWO]);
            x1 = atoi(arg[THREE]);
            width = atoi(arg[FOUR]);
            height = atoi(arg[FIVE]);

            draw_rectangle(y1, x1, width, height, mat, color, line_width, ih);
        }

        if (strstr(command, "draw triangle")) {
        // retin coordonatele punctelor si desenez triunghiul
            int x1 = 0, x2 = 0, x3 = 0, y1 = 0, y2 = 0, y3 = 0;

            y1 = atoi(arg[TWO]);
            x1 = atoi(arg[THREE]);
            y2 = atoi(arg[FOUR]);
            x2 = atoi(arg[FIVE]);
            y3 = atoi(arg[SIX]);
            x3 = atoi(arg[SEVEN]);

            draw_triangle(y1, x1, y2, x2, y3, x3, mat, color, line_width, ih);
        }

        if (strstr(command, "fill")) {
        // retin coordonatele punctului de start si fac fill
            int x = 0, y = 0;

            y = atoi(arg[ONE]);
            x = atoi(arg[TWO]);

            fill(y, x, mat, color, ih);
        }

        if (strstr(command, "quit")) {
            // apelez functia wuit care dezaloca memoria
            // opresc programul
            quit(fh, ih, mat);
            break;
        }
    }
}
