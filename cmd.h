DEF_CMD(HLT, 0, 0, {
    return ALL_RIGHT;
})

DEF_CMD(ADD, 1, 0, {
    A = Pop;
    B = Pop;
    Push(A + B);
})

DEF_CMD(SUB, 2, 0, {
    A = Pop;
    B = Pop;
    Push(A - B);
})

DEF_CMD(MUL, 3, 0, {
    A = Pop;
    B = Pop;
    Push(A * B);
})

DEF_CMD(DIV, 4, 0, {
    A = Pop;
    B = Pop;

    if(B == 0)
        return INVALID_ARGS;
    
    Push(A / B);
})

DEF_CMD(OUT, 5, 0, {
    A = Pop;
    printf("%d\n", A);
})

DEF_CMD(IN, 6, 0, {
    scanf("%d", &A);
    Push(A);
})

DEF_CMD(DUP, 7, 0, {
    A = Pop;
    Push(A);
    Push(A);
})

DEF_CMD(JMP, 8, 1, {
    ip = *arg - 1;
})

DEF_CMD(PUSH, 9, 1, {
    Push(*arg);
})

DEF_CMD(POP, 10, 1, {
    if(arg == &ARGREG)
        return INVALID_ARGS;

    A = Pop;
    *arg = A;
})

DEF_CMD(JB, 11, 1, {
    A = Pop;
    B = Pop;

    if(A < B)
        ip = *arg - 1;
})

DEF_CMD(JBE, 12, 1, {
    A = Pop;
    B = Pop;

    if(A <= B)
        ip = *arg - 1;
})

DEF_CMD(JA, 13, 1, {
    A = Pop;
    B = Pop;

    if(A > B)
        ip = *arg - 1;
})

DEF_CMD(JAE, 14, 1, {
    A = Pop;
    B = Pop;

    if(A >= B)
        ip = *arg - 1;
})

DEF_CMD(JE, 15, 1, {
    A = Pop;
    B = Pop;

    if(A == B)
        ip = *arg - 1;
})

DEF_CMD(JNE, 16, 1, {
    A = Pop;
    B = Pop;

    if(A != B)
        ip = *arg - 1;
})

DEF_CMD(JE, 15, 1, {
    A = Pop;
    B = Pop;

    if(A == B)
        ip = *arg - 1;
})

DEF_CMD(CALL, 16, 1, {
    
})