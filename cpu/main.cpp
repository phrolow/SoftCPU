#include "cpu.h"

int main(int argc, char* argv[]) {
    if(argc < 2) {
        printf("Too few arguments!");

        exit(1);
    }

    struct Code *code = NULL;
    int err = 0;

    ram = (char*)malloc(RAMSIZE);
    regs = (char*)calloc(NUM_REGS, sizeof(char));

    CleanLogs();

    struct Stack stk1 = StackNew();

    handle(getCode(&code, argv[1]));

    handle(execute(code, &stk1));

    StackDtor(&stk1);

    printf("Alright, yeah");

    return err;
}