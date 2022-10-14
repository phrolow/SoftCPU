#include "asm.h"

int compile(struct text *txt, struct Code *code) {
    assert(txt);

    int arg = 0;
    size_t  line = 0,
            i = 0;
    char* cmd = NULL;

    cmd = (char*)calloc(txt->maxLine + 1, sizeof(char));
    code->bin = (char*)calloc(2 * txt->nLine + 4 + 1, sizeof(char));

    code->bin[0] = 'V';
    code->bin[1] = 'F';
    code->bin[2] = VERSION;
    *((size_t*) (code->bin + 3)) = txt->nLine;

    for(i = SIGN; i < SIGN + (1 + sizeof(int)) * txt->nLine; i++) {
        sscanf(txt->ptrs[line], "%s", cmd);

        if(!stricmp(cmd, "push")) {
            code->bin[i] = PUSH_CMD;

            sscanf(txt->ptrs[line], "%s %d", cmd, &arg);

            *((int*) (code->bin + (++i))) = arg;

            i += sizeof(int) - 1;
        }
        else if(!stricmp(cmd, "add")) {
            code->bin[i] = ADD_CMD;
        }
        else if(!stricmp(cmd, "sub")) {
            code->bin[i] = SUB_CMD;
        }
        else if(!stricmp(cmd, "mul")) {
            code->bin[i] = MUL_CMD;
        }
        else if(!stricmp(cmd, "div")) {
            code->bin[i] = DIV_CMD;
        }
        else if(!stricmp(cmd, "out")) {
            code->bin[i] = OUT_CMD;
        }
        else if(!stricmp(cmd, "hlt")) {
            code->bin[i] = HLT_CMD;

            break;
        }
        else if(!stricmp(cmd, "in")) {
            code->bin[i] = IN_CMD;
            
        }
        else {
            return 1;
        }

        line++;
    }

    code->size = i + 1 - SIGN;

    *((size_t*) (code->bin + 3)) = code->size;
    
    return 0;
}

void assemble(struct Code *code, const char *path) {
    assert(code && path && code->bin);

    FILE *fp  = fopen(path, "wb");

    fwrite(code->bin, sizeof(char), code->size + SIGN, fp); 

    fclose(fp);

    /* fp = fopen(TXTOUT, "w");

    for(size_t i = 4; code[i] != 0; i++) {
        if(i == 1)
            fprintf(fp, "%d %d\n", code[i], code[++i]);
        else
            fprintf(fp, "%d\n", code[i]);
    }

    fprintf(fp, "%d\n", 0);
    
    fclose(fp); */
}