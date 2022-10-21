#include "asm.h"

#define DEF_CMD(name, num, argc, ...)                       \
    if(!stricmp(cmd, #name)) {                              \
        code->bin[ip++] = name##_CMD;                       \
                                                            \
        fprintf(logfile, "Binary of command:\n");                \
        fwrite(code->bin + ip - 1, 1, sizeof(char), logfile);    \
        fprintf(logfile, "\n");                                  \
                                                            \
        putArgs(txt->ptrs[line], code->bin, &ip, argc);     \
                                                            \
    }                                                       \
    else

int compile(struct text *txt, struct Code *code) {
    assert(txt);

    size_t  line = 0,
            ip = 0,
            nLabel = 0;
    char *cmd = NULL,
         *lname = NULL;
    
    cmd = (char*)calloc(txt->maxLine + 1, sizeof(char));
    code->bin = (char*)calloc(2 * txt->nLine + 4 + 1, sizeof(char));
    lname = (char*)calloc(LABELSIZE, sizeof(char));

    code->bin[0] = 'V';
    code->bin[1] = 'F';
    code->bin[2] = VERSION;
    *((size_t*) (code->bin + 3)) = txt->nLine;

    ip = SIGNSIZE;

    for(line = 0; line < txt->nLine; line++) {
        fprintf(logfile, "\n\nAssembling line %llu: %s\n", line + 1, txt->ptrs[line]);

        if(txt->ptrs[line][0] == ':') {
            sscanf(txt->ptrs[line], ":%s", lname);

            labels[nLabel].ptr = (int) (ip - SIGNSIZE);
            strcpy(labels[nLabel].name, lname);

            nLabel++;

            fprintf(logfile, "Label %s(%d)", lname, labels[nLabel].ptr);
        }
        else {
            sscanf(txt->ptrs[line], "%s", cmd);

            #include "../cmd.h"
            /* else */ {
                return 1;
            }
        }
    }

    code->size = ip - SIGNSIZE;

    *((size_t*) (code->bin + 3)) = code->size;
    
    fprintf(logfile, "SUCCESS! Assembled %llu bytes\n", code->size);

    return 0;
}

void assemble(struct Code *code, const char *path) {
    assert(code && path && code->bin);

    FILE *fp  = fopen(path, "wb");

    fwrite(code->bin, sizeof(char), code->size + SIGNSIZE, fp); 

    fclose(fp);
}

int putArgs(const char *line, char *bin, size_t *ip, int argc) {
    ON_DEBUG_MODE(size_t ipcpy = *ip);

    size_t  len = 0,
            ptr = 0;
    int     narg = 0,
            imm = 0;
    char    *cmd = NULL,
            *argv[argc + 1] = {},
            *lname = NULL,
            flag = 0;

    cmd = (char*)calloc(strlen(line) + 1, sizeof(char));
    lname = (char*)calloc(LABELSIZE + 1, sizeof(char));
    len = strlen(line);

    for(size_t i = 0; i < len; i++) {
        if(!isspace(line[i])) {
            cmd[ptr++] = line[i];

            if(!flag) {
                if(narg > argc + 1)
                    return 1;

                argv[narg] = &cmd[ptr - 1];

                narg++;

                flag = 1;
            }
        }
        
        if(flag && isspace(line[i])) {
                cmd[ptr++] = '\0';

                flag = 0;
            }
        
        if(line[i] == ';') {
            cmd[ptr] = '\0';

            break;
        }
    }
    
    ON_DEBUG_MODE(fprintf(logfile, "Recognized as \"%s\" (%d args)\n", argv[0], argc));

    if((!stricmp(argv[0], "jmp") || !stricmp(argv[0], "call")) && (argv[1][0] == ':' || !atoi(argv[1]))) {
        bin[*ip - 1] |= ARG_IMMED;

        *((int *) (bin + *ip)) = -1;

        if(argv[1][0] == ':')
            argv[1] += 1;

        for(size_t i = 0; i <= MAXLABELS; i++) {
            if(!stricmp(argv[1], labels[i].name))
                *((int *) (bin + *ip)) = labels[i].ptr;
        }

        *ip += sizeof(int);
    }
    else
        for(int i = 1; i <= argc; i++) {
            char reg = 7;

            if(sscanf(argv[i], "[%s]", argv[i])) {
                bin[*ip - 1] |= ARG_RAM;

                fprintf(logfile, "RAM arg\n");
            }

            if(strchr(argv[i], '+')) {
                sscanf(argv[i], "r%cx+%d", &reg, &imm);

                bin[*ip - 1] |= (ARG_IMMED | ARG_REG);

                bin[*ip] = (reg - 'a' + 1);

                *ip += (sizeof(char));

                *((int*) (bin + *ip)) = imm;

                *ip += (sizeof(int));

                ON_DEBUG_MODE(fprintf(logfile, "IMM & REG arg\n"));
            }
            else if(argv[i][0] == 'r' && argv[i][2] == 'x' && argv[i][3] == '\0') {
                sscanf(argv[i], "r%cx", &reg);

                bin[*ip - 1] |= ARG_REG;
                
                bin[*ip] = (reg - 'a' + 1);

                *ip += (sizeof(char));

                ON_DEBUG_MODE(fprintf(logfile, "REG arg\n"));
            }
            else if(sscanf(argv[i], "%d", &imm)) {
                bin[*ip - 1] |= ARG_IMMED;

                *((int*) (bin + *ip)) = imm;

                *ip += (sizeof(int));

                ON_DEBUG_MODE(fprintf(logfile, "IMM arg\n"));
            }
            else
                return 1;
        }

    #ifdef DEBUG_MODE
    if(argc) {
        fprintf(logfile, "Assembled binary code (%llu bytes):\n", *ip - ipcpy + 1);
        fwrite(bin + ipcpy - 1, *ip - ipcpy + 1, sizeof(char), logfile);
        fprintf(logfile, "\n");
    }
    #endif

    free(cmd);
    free(lname);

    return 0;
}