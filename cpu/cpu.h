#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include "stack/stack.h"
#include "../softcpu.h"

#define Push(a) StackPush(stk, a)
#define Pop StackPop(stk)

struct Code {
    char* bin;
    size_t size;
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

extern int ARGREG;
extern int *ram;
extern int *regs;

int execute(struct Code *code, struct Stack *stk);

void handle(int err);

int getCode(struct Code *code, const char *path);

int *getArg(char* bin, size_t *ip, int argc);

#endif // CPU_H_INCLUDED