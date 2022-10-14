#include "cpu.h"

int execute(struct Code *code, struct Stack *stk) {
    assert(code && stk);

    Elem_t  IN = 0;

    FILE *fp = fopen(LOGPATH, "ab");
    
    fprintf(fp, "Executing code (size = %llu)\nBin: ", code->size);
    fwrite(code->bin, sizeof(char), code->size, fp);
    fprintf(fp, "\n\n");

    fclose(fp);
    
    for(size_t ip = 0; ip < code->size; ip++) {
        fp = fopen(LOGPATH, "ab");

        fprintf(fp, "EXECUTING COMMAND IP=%llu (%d):\n\n", ip, code->bin[ip]);

        fclose(fp);

        switch(code->bin[ip]) {
            case HLT_CMD:
                break;
            case PUSH_CMD:
                StackPush(stk, *((int*) (code->bin + (++ip))));
                
                ip += sizeof(int) - 1;

                break;
            case ADD_CMD:
                StackPush(stk, StackPop(stk) + StackPop(stk));

                break;
            case SUB_CMD:
                StackPush(stk, StackPop(stk) - StackPop(stk));

                break;
            case MUL_CMD:
                StackPush(stk, StackPop(stk) * StackPop(stk));

                break;
            case DIV_CMD:
                StackPush(stk, StackPop(stk) / StackPop(stk)); //handle

                break;
            case OUT_CMD:
                printf("%d\n", StackPop(stk));

                break;
            case IN_CMD:
                scanf("%d", &IN);

                StackPush(stk, IN);

                break;
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