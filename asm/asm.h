#ifndef ASM_H_INCLUDED
#define ASM_H_INCLUDED

#include <assert.h>
#include <ctype.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "../softcpu.h"

#define DEBUG_MODE

const size_t LABELSIZE = 20;
const size_t MAXLABELS = 50;

const char* const INEXTENSION = "asm";
const char* const OUTEXTENSION = "bin";
const char* const TXTOUT = "a.txt";
const char* const LOGPATH = "asmlog.txt";
const char* const JMPCMDS[] = {
    "JMP",
    "CALL",
    "JA",
    "JAE",
    "JB",
    "JBE",
    "JE",
    "JNE"
}; //cringe
const int NJMPCMDS = 8; //cringe

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

struct Label {
    int ptr;
    char name[LABELSIZE];
};

extern struct Label labels[MAXLABELS];
ON_DEBUG_MODE(extern FILE *logfile);

struct text maketext(char *content, char **ptrs, size_t nChar, size_t nLine, size_t maxLine);

struct text textFromFile(const char *path);

int compile(struct text *txt, struct Code *code);

void assemble(struct Code *code, const char *path);

int putArgs(const char *line, char *bin, size_t *ip, int argc);

int checkjmp(const char *cmd);

void txtDtor(struct text *txt);

#endif