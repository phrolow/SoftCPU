#include "stack.h"

int main() {
    char *bin = NULL;
    int err = 0;

    CleanLogs();

    struct Stack stk1 = StackNew();

    getCode(argv[1]);

    handle(err = execute(code));

    StackDtor(&stk1);

    return err;
}