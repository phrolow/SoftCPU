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
    ip = arg - 1;
})

DEF_CMD(PUSH, 9, 1, {
    Push(arg);
})

DEF_CMD(POP, 10, 1, {
    A = Pop;
    ram[arg] = A;
})