#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include "stack/stack.h"
#include "../softcpu.h"

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

int execute(struct Code *code, struct Stack *stk);

void handle(int err);

int getCode(struct Code **code, const char *path);

int getArgs(char* bin, size_t *ip, int argc, int* argv);

#endif // CPU_H_INCLUDED