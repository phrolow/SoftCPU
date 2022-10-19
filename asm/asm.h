#ifndef ASM_H_INCLUDED
#define ASM_H_INCLUDED

#include <assert.h>
#include <ctype.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys\stat.h>

#include "../softcpu.h"

#define DEBUG_MODE

const size_t LABELSIZE = 20;
const size_t MAXLABELS = 50;

const char* const INEXTENSION = "asm";
const char* const OUTEXTENSION = "bin";
const char* const TXTOUT = "a.txt";
const char* const LOGPATH = "log.txt";

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

int checkfile(const char *path, const char *extension);

int compile(struct text *txt, struct Code *code);

void assemble(struct Code *code, const char *path);

int putArgs(const char *line, char *bin, size_t *ip, int argc);

#endif