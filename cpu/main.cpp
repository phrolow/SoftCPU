#include "cpu.h"

int main(int argc, char* argv[]) {
    CleanLogs();
    
    if(argc < 2) {
        printf("Too few arguments!");

        exit(1);
    }

    struct Code code = { NULL, 0};
    int err = 0;
    struct Stack stk1 = StackNew();
    struct Stack callstk = StackNew();

    struct CPU cpu = { 0, NULL, NULL, &stk1, &callstk};

    cpu.ram = (int*)malloc(RAMSIZE * sizeof(int));
    cpu.regs = (int*)calloc(NUM_REGS, sizeof(int));
    cpu.ARGREG = 0;

    handle(getCode(&code, argv[1]));

    handle(execute(&code, &cpu));

    StackDtor(&stk1);
    StackDtor(&callstk);

    printf("Alright, yeah");

    return err;
}