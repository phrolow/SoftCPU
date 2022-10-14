#ifndef ASM_H_INCLUDED
#define ASM_H_INCLUDED

#include <assert.h>
#include <ctype.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys\stat.h>

#include "../softcpu.h"

struct text {
    char *content;
    char **ptrs;
    size_t  nChar,
            nLine,
            maxLine;
};

struct Code {
    char *bin;
    size_t  size;
};

const char* const INEXTENSION = "asm";
const char* const OUTEXTENSION = "bin";
const char* const TXTOUT = "a.txt";

struct text maketext(char *content, char **ptrs, size_t nChar, size_t nLine, size_t maxLine);

struct text textFromFile(const char *path);

int checkfile(const char *path, const char *extension);

int compile(struct text *txt, struct Code *code);

void assemble(struct Code *code, const char *path);

#endif