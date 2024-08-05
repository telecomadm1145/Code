        .ident "TYPE:"

        .ident "CODE:large"   
        .globl   strcpy_nn
        .type    strcpy_nn,@function
        .globl   memcpy_nn
        .type    memcpy_nn,@function
    strcpy_nn:
        PUSH XR8
        PUSH BP
        MOV ER8, ER0
        MOV ER10, ER2
        MOV BP, ER0
    .l_00A:
        L R0, [ER10]
        ST R0, [BP]
        ADD ER10, #1
        ADD BP, #1
        MOV R0, R0
        BNE .l_00A
        MOV ER0, ER8
        POP BP
        POP XR8
        RT

    memcpy_nn:
        PUSH FP
        MOV FP, SP
        PUSH BP
        PUSH ER8
        PUSH ER4
        MOV ER8, ER0
        MOV ER4, ER0
        MOV BP, ER2
        BAL .l_024
    .l_012:
        MOV ER0, ER4
        L R2, [BP]
        ST R2, [ER4]
        ADD ER0, #1
        MOV ER4, ER0
        ADD BP, #1
        L ER0, +2[FP]
        ADD ER0, #-1
        ST ER0, +2[FP]
    .l_024:
        MOV ER0, #00
        L ER2, +2[FP]
        CMP ER0, ER2
        BLT .l_012
        MOV ER0, ER8
        POP ER4
        POP ER8
        POP BP
        MOV SP, FP
        POP FP
        RT

