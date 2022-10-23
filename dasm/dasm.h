#ifndef DASM_H_INCLUDED
#define DASM_H_INCLUDED

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../softcpu.h"

struct Code {
    char* bin;
    size_t size;
};

enum DasmError {
    ALL_RIGHT,
    INVALID_SIGNATURE,
    INVALID_SIZE,
    INVALID_COMMAND,
    INVALID_ARGS
};

const char* const LOGPATH = "log.txt";

int getCode(struct Code *code, const char *path);

int disassemble(struct Code *code, const char *path);

char *getArgs(char* bin, size_t *ip, int argc);

void handle(int err);

void CleanLogs();

#endif