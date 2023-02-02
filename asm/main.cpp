#include "asm.h"

struct Label labels[MAXLABELS] = {};
FILE *logfile = NULL;

int main(int argc, char *argv[]) {
    ON_DEBUG_MODE(logfile = fopen(LOGPATH, "wb"));

    if(argc < 3) {
        printf("Too few arguments");

        return 1;
    }

    struct Code code = { NULL, 0 };
    struct text prog = { NULL, NULL, 0, 0, 0 };

    prog = textFromFile(argv[1]);

    if(compile(&prog, &code) || compile(&prog, &code)) {
        printf("Lmfao guy\n");

        return 1;
    }

    assemble(&code, argv[2]);

    printf("Alright, yeah!");

    ON_DEBUG_MODE(fclose(logfile));

    txtDtor(&prog);
    free(code.bin);
    
    return 0;
}