#include "stack.h"

void StackDump_(struct Stack *stk, const char *func, const char *file, size_t line) {
    struct StackInfo* info = NULL;
    FILE *fp = NULL;

    info = &(stk->info);
    fp = fopen(LOGPATH, "a");

    fprintf(fp, "%s at %s(%llu)\n", func, file, line);
    fprintf(fp, "Stack[%p] (ok) at %s at %s(%llu) {\n", stk, info->func, info->file, info->line);
    fprintf(fp, "\tSize = %llu\n", stk->Size);
    fprintf(fp, "\tcapacity = %llu\n", stk->capacity);
    fprintf(fp, "\tdata[%p] {\n", info->name);
    
    for(size_t i = 0; i < stk->capacity; i++) {
        fprintf(fp, "\t\t");

        if(i < stk->Size)
            fprintf(fp, "*");
        
        fprintf(fp, "[%llu] = %d", i, stk->data[i]);

        if(stk->data[i] == POISON)
            fprintf(fp, " (POISON)");
        
        fprintf(fp, "\n");
    }

    fprintf(fp, "\t}\n}\n");

    fclose(fp);

    return;
}

int StackError_(struct Stack *stk, const char* func, const char* file, size_t line) {
    int err = 0;

    long oldstackhash = stk->stackhash;
    long olddatahash = stk->datahash;

    StackHash(stk);
    StackDataHash(stk);

    err |= CHECK(stk, NULL_POINTER);

    err |= CHECK(stk->data, NULL_DATA);
    
    err |= CHECK(stk->Size < BIG_UNS, BAD_SIZE);
    
    err |= CHECK(stk->capacity < BIG_UNS && stk->capacity > 0, BAD_CAPACITY);

    err |= CHECK(stk->canary0 == stk->canary1, CORRUPTED_CANARIES);

    err |= CHECK(stk->stackhash == oldstackhash, CORRUPTED_STACK);

    err |= CHECK(checkdatacanaries(stk), CORRUPTED_DATA_CANARIES);

    err |= CHECK(stk->datahash == olddatahash, CORRUPTED_DATA);

    perror_(err, file, func, line);
    
    return err;
}

void CleanLogs() {
    fclose(fopen(LOGPATH, "w"));
}

void perror_(int err, const char* file, const char* func, size_t line) {
    int binerr = binary(err);

    FILE* fp = fopen(LOGPATH, "a");

    if(err)
        fprintf(fp, "ERROR 0b%08d in %s at %s(%llu)\n\n", binerr, func, file, line);
    else
        fprintf(fp, "(OK)\n\n");

    fclose(fp);

    return;
}