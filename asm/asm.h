#ifndef ASM_H_INCLUDED
#define ASM_H_INCLUDED

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys\stat.h>

//#include <TXLib.h>

struct text {
    char *content;
    char **ptrs;
    size_t  nChar,
            nLine,
            maxLine;
};

const char* const INEXTENSION = "asm";
const char* const OUTEXTENSION = "bin";
const char* const TXTOUT = "a.txt";
const char VERSION = 2;

enum COMMAND {
    HLT_CMD,
    PUSH_CMD,
    ADD_CMD,
    SUB_CMD,
    MUL_CMD,
    DIV_CMD,
    OUT_CMD,
    IN_CMD
};

struct text maketext(char *content, char **ptrs, size_t nChar, size_t nLine, size_t maxLine);

struct text textFromFile(const char *path);

int checkfile(const char *path, const char *extension);

int compile(struct text *txt, char **code);

void assemble(char *code, const char *path);

#endif