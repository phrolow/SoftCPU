#include "asm.h"

struct Label labels[MAXLABELS] = {};
FILE *logfile = NULL;

int main(int argc, char *argv[]) {
    ON_DEBUG_MODE(logfile = fopen(LOGPATH, "wb"));

    if(argc < 3) {
        printf("Too few arguments");

        return 1;
    }

    /* if(!checkfile(argv[1], INEXTENSION)) {
        printf("Invalid input file");

        return 1;
    }

    if(!checkfile(argv[2], OUTEXTENSION)) {
        printf("Invalid output file");

        return 1;
    } */

    struct Code code = { NULL, 0 };
    struct text prog = { NULL, NULL, 0, 0, 0 };

    prog = textFromFile(argv[1]);

    if(compile(&prog, &code)) {
        printf("ТЫ ЧЕ ДУРАК БЛЯТЬ!?");

        return 1;
    }

    assemble(&code, argv[2]);

    printf("Alright, yeah!");

    ON_DEBUG_MODE(fclose(logfile));
    
    return 0;
}