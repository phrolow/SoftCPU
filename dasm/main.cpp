#include "dasm.h"

int main(int argc, char* argv[]) {
    CleanLogs();
    
    if(argc < 2) {
        printf("Too few arguments!");

        exit(1);
    }

    struct Code code = { NULL, 0};
    int err = 0;

    handle(getCode(&code, argv[1]));
    handle(disassemble(&code, argv[2]));

    printf("Alright, yeah");

    return err;
}

void CleanLogs() {
    fclose(fopen(LOGPATH, "wb"));
}