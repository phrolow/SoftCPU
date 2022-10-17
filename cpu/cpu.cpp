#include "cpu.h"

#define DEF_CMD(name, num, argc, ...)                   \
    case name##_CMD:                                    \
                                                        \
    arg = getArg(code->bin, &ip, argc, arg);                \
                                                        \
    __VA_ARGS__

int execute(struct Code *code, struct Stack *stk) {
    assert(code && stk);

    Elem_t  IN = 0;
    Elem_t  A = 0;
    Elem_t  B = 0;

    FILE *fp = fopen(LOGPATH, "ab");
    
    fprintf(fp, "Executing code (size = %llu)\nBin: ", code->size);
    fwrite(code->bin, sizeof(char), code->size, fp);
    fprintf(fp, "\n\n");

    fclose(fp);
    
    for(size_t ip = 0; ip < code->size; ip++) {
        fp = fopen(LOGPATH, "ab");

        fprintf(fp, "EXECUTING COMMAND IP=%llu (%d):\n\n", ip, code->bin[ip]);

        fclose(fp);

        int arg = 0;

        switch(code->bin[ip] % 0b100000) {
            #include "../cmd.h"
            #undef DEF_CMD
            default:
                return INVALID_COMMAND;
        }
    }
    
    return ALL_RIGHT;
}

void handle(int err) {
    if(err) {
        FILE *fp = fopen(LOGPATH, "a");
        
        fprintf(fp, "CPU error %d", err);
        
        fclose(fp);

        abort();
    }
}

int getCode(struct Code **code, const char *path) {
    struct Code cod = { NULL, 0 };

    *code = &cod;

    char sign[3 + sizeof(size_t) + 1] = {};

    FILE *fp = fopen(path, "rb");
    
    fread(sign, sizeof(char), 3 + sizeof(size_t), fp);

    if(sign[0] != 'V' || sign[1] != 'F' || sign[2] != VERSION) {
        return INVALID_SIGNATURE;
    }
    
    (*code)->size = *((size_t *) (sign + 3));

    (*code)->bin = (char*)calloc((*code)->size, sizeof(char));

    fread((*code)->bin, sizeof(char), (*code)->size, fp);
    
    fclose(fp);

    fp = fopen(LOGPATH, "wb");
    
    fprintf(fp, "GOT CODE (SIZE = %llu) WITH SIGNATURE: \n", (*code)->size);
    fwrite(sign, sizeof(char), 3 + sizeof(size_t), fp);
    fprintf(fp, "\n");
    
    fclose(fp);
    
    return 0;
}

int getArg(char* bin, size_t *ip, int argc) {
    int arg = 0;

    if(argc == 0)
        return;

    char cmd = bin[*ip++];

    if(cmd & ARG_IMMED) {
        arg += *((int*) (bin + *ip));

        *ip += sizeof(int);
    }
    if(cmd & ARG_REG) {
        arg += regs[bin[*ip]];

        *ip += sizeof(char);
    }
    if(cmd & ARG_RAM)
        arg = ram[arg];

    return arg;
}