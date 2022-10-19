#include "cpu.h"

int *ram;
int *regs;

int main(int argc, char* argv[]) {
    CleanLogs();
    
    if(argc < 2) {
        printf("Too few arguments!");

        exit(1);
    }

    struct Code code = { NULL, 0};
    int err = 0;
    struct Stack stk1 = StackNew();

    ram = (int*)malloc(RAMSIZE * sizeof(int));
    regs = (int*)calloc(NUM_REGS, sizeof(int));

    handle(getCode(&code, argv[1]));

    handle(execute(&code, &stk1));

    StackDtor(&stk1);

    printf("Alright, yeah");

    return err;
}