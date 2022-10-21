#include "cpu.h"

int *ram,
    *regs,
    ARGREG;
struct Stack callstk;

int main(int argc, char* argv[]) {
    CleanLogs();
    
    if(argc < 2) {
        printf("Too few arguments!");

        exit(1);
    }

    struct Code code = { NULL, 0};
    int err = 0;
    struct Stack stk1 = StackNew();
    callstk = StackNew();

    ram = (int*)malloc(RAMSIZE * sizeof(int));
    regs = (int*)calloc(NUM_REGS, sizeof(int));
    ARGREG = 0;

    handle(getCode(&code, argv[1]));

    handle(execute(&code, &stk1));

    StackDtor(&stk1);
    StackDtor(&callstk);

    printf("Alright, yeah");

    return err;
}