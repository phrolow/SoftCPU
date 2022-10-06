#include "asm.h"

struct text maketext(char *content, char **ptrs, size_t nChar, size_t nLine, size_t maxLine) {
    struct text newText = { NULL, NULL, 0, 0, 0 };

    newText.content = content;
    newText.ptrs = ptrs;
    newText.nChar = nChar;
    newText.nLine = nLine;
    newText.maxLine = maxLine;

    return newText;
}

struct text textFromFile(const char *path) {
    assert(path);

    FILE *fp = NULL;
    size_t  currentLine = 0,
            i = 0,
            iLine = 0,
            nChar = 0,
            nLine = 0,
            maxLine = 0;
    char **ptrs = NULL;
    char *content = NULL;
    struct stat stats;

    fp = fopen(path, "r");

    fstat(fileno(fp), &stats);

    nChar = stats.st_size / sizeof(char);

    content = (char*)calloc(nChar + 1, sizeof(char));

    nChar = fread(content, sizeof(char), nChar, fp);

    fclose(fp);

    content = (char*)realloc(content, nChar + 1);

    for(i = 0; i < nChar; i++)
        if(content[i] == '\n') {
            nLine++;

            content[i] = '\0';
        }

    nLine++;

    content[i + 1] = '\0';

    ptrs = (char**)calloc(nLine + 1, sizeof(char*));

    ptrs[0] = &(content[0]);
    iLine++;

    for(i = 0; i < nChar; i++) {
        currentLine++;

        if(content[i] == '\0') {
            ptrs[iLine] = &(content[i + 1]);

            iLine++;

            if(currentLine > maxLine)
                maxLine = currentLine;

            currentLine = 0;
        }
    }

    return maketext(content, ptrs, nChar, nLine, maxLine);
}

int checkfile(const char* path, const char *extension) {
    assert(path && extension);

    char* ptr = NULL;

    ptr = strchr(path, '.');

    if(ptr) {
        ptr++;

        if(strcmp(ptr, extension))
            return 1;
    }

    return 0;
}

int compile(struct text *txt, char **code) {
    assert(txt);

    size_t line = 0;
    char* cmd = NULL;

    cmd = (char*)calloc(txt->maxLine + 1, sizeof(char));
    (*code) = (char*)calloc(2 * txt->nLine + 4 + 1, sizeof(char));

    (*code)[0] = 'V';
    (*code)[1] = 'F';
    (*code)[2] = VERSION;
    (*code)[3] = (char) txt->nLine;

    for(size_t i = 4; i < 2 * txt->nLine + 4; i++) {
        sscanf(txt->ptrs[line], "%s", cmd);

        if(!stricmp(cmd, "push")) {
            (*code)[i] = PUSH_CMD;
            
            sscanf(txt->ptrs[line], "%s %d", cmd, (*code + (++i)));
        }
        else if(!stricmp(cmd, "add")) {
            (*code)[i] = ADD_CMD;
        }
        else if(!stricmp(cmd, "sub")) {
            (*code)[i] = SUB_CMD;
        }
        else if(!stricmp(cmd, "mul")) {
            (*code)[i] = MUL_CMD;
        }
        else if(!stricmp(cmd, "div")) {
            (*code)[i] = DIV_CMD;
        }
        else if(!stricmp(cmd, "out")) {
            (*code)[i] = OUT_CMD;
        }
        else if(!stricmp(cmd, "hlt")) {
            (*code)[i] = HLT_CMD;

            break;
        }
        else if(!stricmp(cmd, "in")) {
            (*code)[i] = IN_CMD;
        }
        else {
            return 1;
        }

        line++;
    }
    
    return 0;
}

void assemble(char *code, const char *path) {
    assert(code && path);

    FILE *fp = fopen(path, "w");
    
    for(size_t i = 0; code[i] != 0; i++) {
        if(i == PUSH_CMD)
            fprintf(fp, "%c%c", code[i], code[i++]);
        else
            fprintf(fp, "%c", code[i]);
    }

    fprintf(fp, "%c", 0);
    
    fclose(fp);

    fp = fopen(TXTOUT, "w");

    for(size_t i = 4; code[i] != 0; i++) {
        if(i == 1)
            fprintf(fp, "%d %d\n", code[i], code[i++]);
        else
            fprintf(fp, "%d\n", code[i]);
    }

    fprintf(fp, "%d\n", 0);
    
    fclose(fp);
}