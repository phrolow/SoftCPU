#include "stack.h"

void StackDump_(struct Stack *stk, const char *func, const char *file, size_t line) {
    struct StackInfo* info = NULL;
    FILE *fp = NULL;

    info = &(stk->info);
    fp = fopen(LOGPATH, "a");

    fprintf(fp, "%s at %s(%u)\n", func, file, line);
    fprintf(fp, "Stack[%lx] (ok) at %s at %s(%u) {\n", (long) stk, info->func, info->file, info->line);
    fprintf(fp, "\tSize = %u\n", stk->Size);
    fprintf(fp, "\tcapacity = %u\n", stk->capacity);
    fprintf(fp, "\tdata[%lx] {\n", (long) info->name);
    
    for(size_t i = 0; i < stk->capacity; i++) {
        fprintf(fp, "\t\t");

        if(i <= stk->Size)
            fprintf(fp, "*");
        
        fprintf(fp, "[%u] = %c", i, stk->data[i]);

        if(stk->data[i] == POISON)
            fprintf(fp, " (POISON)");
        
        fprintf(fp, "\n");
    }

    fprintf(fp, "\t}\n}\n\n");

    fclose(fp);

    return;
}

int StackError_(struct Stack *stk, const char* func, const char* file, size_t line) {
    int err = 0;

    err |= CHECK(stk, NULL_POINTER);

    err |= CHECK(stk->data, NULL_DATA);
    
    err |= CHECK(stk->Size < BIG_UNS, BAD_SIZE);
    
    err |= CHECK(stk->capacity < BIG_UNS && stk->capacity > 0, BAD_CAPACITY);

    err |= CHECK(stk->canary0 == stk->canary1, CORRUPTED_CANARIES);

    err |= CHECK(stk->stackhash == StackHash(stk), CORRUPTED_STACK);

    err |= CHECK(checkdatacanaries(stk), CORRUPTED_DATA_CANARIES);

    err |= CHECK(stk->datahash == StackDataHash(stk), CORRUPTED_DATA);

    if(err)
        perror_(err, file, func, line);

    StackHash(stk);
    
    return err;
}

void CleanLogs() {
    fclose(fopen(LOGPATH, "w"));
}

void perror_(int err, const char* file, const char* func, size_t line) {
    FILE* fp = fopen(LOGPATH, "a");

    fprintf(fp, "0b%08d in %s at %s(%d)\n\n", binary(err), func, file, line);

    fclose(fp);

    return;
}