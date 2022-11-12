#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include "stack/stack.h"
#include "../softcpu.h"

#define Push(a) StackPush(cpu->stk, a)
#define Pop StackPop(cpu->stk)

struct Code {
    char* bin;
    size_t size;
};

struct CPU {
    int ARGREG;
    int *ram;
    int *regs;
    struct Stack *stk;
    struct Stack *callstk;
};


enum CpuError {
    ALL_RIGHT,
    INVALID_SIGNATURE,
    INVALID_SIZE,
    INVALID_COMMAND,
    INVALID_ARGS
};

const size_t RAMSIZE = 0x100;
const size_t NUM_REGS = 4;

int execute(struct Code *code, struct CPU *cpu);

void handle(int err);

int getCode(struct Code *code, const char *path);

int *getArg(struct CPU *cpu, char* bin, size_t *ip, int argc);

#endif // CPU_H_INCLUDED