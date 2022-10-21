#include "cpu.h"

#define DEF_CMD(name, num, argc, ...)                   \
    case name##_CMD:                                    \
                                                        \
    arg = getArg(code->bin, &ip, argc);                 \
                                                        \
    __VA_ARGS__                                         \
                                                        \
    break;

int execute(struct Code *code, struct Stack *stk) {
    assert(code && stk);

    Elem_t  A = 0;
    Elem_t  B = 0;
    int *arg = NULL;

    FILE *fp = fopen(LOGPATH, "ab");
    
    fprintf(fp, "Executing code (size = %llu)\nBin: ", code->size);
    fwrite(code->bin, sizeof(char), code->size, fp);
    fprintf(fp, "\n\n");

    fclose(fp);
    
    for(size_t ip = 0; ip < code->size; ip++) {
        fp = fopen(LOGPATH, "ab");

        fprintf(fp, "EXECUTING COMMAND IP=%llu (%x):\n\n", ip, code->bin[ip]);

        fclose(fp);

        switch(code->bin[ip] % 0x20) {
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

int getCode(struct Code *code, const char *path) {
    char sign[3 + sizeof(size_t) + 1] = {};

    FILE *fp = fopen(path, "rb");
    
    fread(sign, sizeof(char), 3 + sizeof(size_t), fp);

    if(sign[0] != 'V' || sign[1] != 'F' || sign[2] != VERSION) {
        return INVALID_SIGNATURE;
    }
    
    code->size = *((size_t *) (sign + 3));

    code->bin = (char*)calloc(code->size, sizeof(char));

    fread(code->bin, sizeof(char), code->size, fp);
    
    fclose(fp);

    fp = fopen(LOGPATH, "ab");
    
    fprintf(fp, "GOT CODE (SIZE = %llu).\nSignature: \n", code->size);
    fwrite(sign, sizeof(char), SIGNSIZE, fp);
    fprintf(fp, "\nBin:\n");
    fwrite(code->bin, sizeof(char), code->size, fp);
    fprintf(fp, "\n\n");
    
    fclose(fp);
    
    return 0;
}

int *getArg(char* bin, size_t *ip, int argc) {
    int *ptr = NULL;
    ARGREG = 0;

    if(argc == 0)
        return 0;

    FILE *fp = fopen(LOGPATH, "ab");
    
    char cmd = bin[(*ip)++];

    if(cmd & ARG_REG) {
        ARGREG += regs[(int) bin[*ip]];
        ptr = regs + (int) bin[*ip];

        fprintf(fp, "IP[%llu] Got register arg: %d (from reg r%cx)\n", *ip, regs[(int) bin[*ip]], bin[*ip] + 'a' - 1);

        *ip += sizeof(char);
    }
    if(cmd & ARG_IMMED) {
        ARGREG += *((int*) (bin + *ip));
        ptr = &ARGREG;

        fprintf(fp, "IP[%llu] Got immed arg: %d\n", *ip, *((int*) (bin + *ip)));

        *ip += sizeof(int);
    }
    if(cmd & ARG_RAM) {
        fprintf(fp, "Arg is from RAM (RAM[%d]) = %d\n", ARGREG, ram[ARGREG]);

        ptr = ram + ARGREG;
        ARGREG = *ptr;
    }
    
    (*ip)--;

    fprintf(fp, "Got arg: %d\n\n", ARGREG);

    fclose(fp);

    return ptr;
}