#include "dasm.h"

#define DEF_CMD(name, num, argc, ...)                                               \
    case name##_CMD:                                                                \
        fprintf(fp, "%s ", #name);                                                  \
                                                                                    \
        if(argc > 0)                                                                \
            fprintf(fp, "%s", getArgs(code->bin, &ip, argc));                       \
                                                                                    \
        fprintf(fp, "\n");                                                          \
                                                                                    \
        break;   

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

int disassemble(struct Code *code, const char *path) {
    assert(code);

    FILE *fp,
         *logfile;

    logfile = fopen(LOGPATH, "ab");
    fp = fopen(path, "wb");
    
    fprintf(logfile, "Disassembling code (size = %llu)\nBin: ", code->size);
    fwrite(code->bin, sizeof(char), code->size, logfile);
    fprintf(logfile, "\n\n");

    for(size_t ip = 0; ip < code->size; ip++) {
        switch(code->bin[ip] % 0x20) {
            #include "../cmd.h"
            #undef DEF_CMD
            default:
                return INVALID_COMMAND;
        }
    }
    
    fclose(logfile);
    fclose(fp);

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

char *getArgs(char* bin, size_t *ip, int argc) {
    assert(argc);

    char    *args = NULL,
            cmd = 0;

    args = (char*)calloc(20, sizeof(char));
    FILE *fp = fopen(LOGPATH, "ab");
    
    cmd = bin[(*ip)++];

    if(cmd & ARG_REG) {
        sprintf(args, "r%cx", bin[*ip] + 'a' - 1);

        fprintf(fp, "IP[%llu] Read register arg: r%cx (REG[%d])\n", *ip, bin[*ip] + 'a' - 1, bin[*ip]);

        *ip += sizeof(char);
    }
    if(cmd & ARG_IMMED) {
        if(cmd & ARG_REG)
            args[strlen(args)] = '[';

        itoa(*((int*) (bin + *ip)), args + strlen(args), 10);

        fprintf(fp, "IP[%llu] Got immed arg: %d\n", *ip, *((int*) (bin + *ip)));

        *ip += sizeof(int);
    }
    if(cmd & ARG_RAM) {
        fprintf(fp, "Arg is from RAM\n");

        strcpy(args, args + 1);
        args[0] = ']';
        strcat(args, "]");
    }
    
    (*ip)--;

    fprintf(fp, "Dissassembled arg: %s\n\n", args);

    fclose(fp);

    return args;
}