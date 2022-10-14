#include "cpu.h"

long StackHash(struct Stack *stk){
    stk->stackhash = 0;

    stk->stackhash = hash(stk, sizeof(*stk));

    return stk->stackhash;
}

long StackDataHash(struct Stack *stk) {
    stk->datahash = hash(stk->data, stk->capacity * sizeof(Elem_t));

    return stk->datahash;
}

long hash(void* p, size_t size) {
    long res = 0;
    char* ptr = NULL;

    ptr = (char*)p;

    for(size_t i = 0; i < size; i++) {
        res = ((res << 5) + res) + ptr[i];
    }

    return res;
}