#include "stack.h"

int binary(int n) {
    int res = 0, ten = 1;

    while(n > 0) {
        res += n % 2 * ten;
        
        ten *= 10;

        n /= 2;
    }

    return res;
}

void* recalloc(void* ptr, size_t num, size_t Size) {
    assert(ptr && num && Size);

    ptr = realloc(ptr, num * Size);

    return ptr;
}