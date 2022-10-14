DEF_CMD(HLT, 0, 0, {
    break;
})

DEF_CMD(ADD, 1, 0, {
    A = POP;
    B = POP;
    PUSH(A + B);
})

DEF_CMD(SUB, 2, 0, {
    A = POP;
    B = POP;
    PUSH(A - B);
})

DEF_CMD(MUL, 3, 0, {
    A = POP;
    B = POP;
    PUSH(A * B);
})

DEF_CMD(DIV, 4, 0, {
    A = POP;
    B = POP;

    if(B == 0)
        return INVALID_ARGS;
    
    PUSH(A / B);
})

DEF_CMD(OUT, 5, 0, {
    A = POP;
    printf("%d\n", A);
})

DEF_CMD(IN, 6, 0, {
    scanf("%d", &A);
    PUSH(A);
})

DEF_CMD(DUP, 7, 0, {
    A = POP;
    PUSH(A);
    PUSH(A);
})

DEF_CMD(JMP, 8, 1, {
    ip = args[0];
})

DEF_CMD(PUSH, 9, 1, {
    PUSH(args[0]);
})