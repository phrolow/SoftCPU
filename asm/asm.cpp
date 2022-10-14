#include "asm.h"

#define DEF_CMD(name, num, argc, ...)                   \
    if(!stricmp(cmd, #name)) {                          \
        code->bin[ip++] = name##_CMD;                      \
                                                        \
        putArgs(txt->ptrs[line], code->bin, &ip, argc);   \
    }                                                   \
    else

int compile(struct text *txt, struct Code *code) {
    assert(txt);

    size_t  line = 0,
            ip = 0;
    char* cmd = NULL;

    cmd = (char*)calloc(txt->maxLine + 1, sizeof(char));
    code->bin = (char*)calloc(2 * txt->nLine + 4 + 1, sizeof(char));

    code->bin[0] = 'V';
    code->bin[1] = 'F';
    code->bin[2] = VERSION;
    *((size_t*) (code->bin + 3)) = txt->nLine;

    for(line = 0; line < txt->nLine; line++) {
        sscanf(txt->ptrs[line], "%s", cmd);

        #include "../cmd.h"
        /* else */ {
            return 1;
        }
    }

    code->size = ip + 1 - SIGNSIZE;

    *((size_t*) (code->bin + 3)) = code->size;
    
    return 0;
}

void assemble(struct Code *code, const char *path) {
    assert(code && path && code->bin);

    FILE *fp  = fopen(path, "wb");

    fwrite(code->bin, sizeof(char), code->size + SIGNSIZE, fp); 

    fclose(fp);
}

int putArgs(const char *line, char *bin, size_t *ip, int argc) {
    size_t  len = 0,
            ptr = 0;
    int     narg = 0;
    char    *cmd = NULL,
            *argv[argc + 1] = {},
            flag = 0;

    cmd = (char*)calloc(strlen(line) + 1, sizeof(char));
    len = strlen(line);

    for(size_t i = 0; i < len; i++) {
        if(isalpha(line[i])) {
            cmd[ptr++] = line[i];

            if(!flag) {
                if(narg > argc)
                    return 1;

                argv[narg] = &cmd[ptr - 1];

                narg++;

                flag = 1;
            }
        }
        
        if(flag && (line[i] == ' ' || line[i] == '\t')) {
                cmd[ptr++] = '\0';

                flag = 0;
            }
        
        if(line[i] == ';') {
            cmd[ptr] = '\0';

            break;
        }
    }

    for(int i = 1; i <= argc; i++)
        *((int*) (bin + *ip)) = (atoi(argv[i]));

    *ip += sizeof(int) - 1;

    return 0;
}