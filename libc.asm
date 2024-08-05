        .ident "TYPE:"

        .ident "CODE:large"   
        .globl   strcpy_nn
        .type    strcpy_nn,@function
        .globl   memcpy_nn
        .type    memcpy_nn,@function
        .globl   memzero_n
        .type    memzero_n,@function
        .globl memset_n
        .type memset_n,@function
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
        push er4
        add sp,2
        pop er4
        add sp,-4
        lea [er2]
    .loop5:
        l er2,[ea+]
        st er2,[er0]
        add er0, 2
        add er4, -2
        bne .loop5
        
        pop er4
        rt

    memzero_n:
        push r4
        mov r4,0
        lea [er0]
    .loop:
        st r4,[ea+]
        add er2,-1
        bne .loop
        
        pop r4
        rt

    memzero_w_n:
        push er4
        mov er4, 0
        lea [er0]
        tb r0 1 ; 地址低位
        beq .pass0
        ; 不为0，对齐一下
        st r4, [ea+]
        add er2, -1
    .pass0:
        rb r2, 0 ; 字节数低位
        beq .pass1
        ; 不为0，记录一下
        mov r5, 1
        add er2, -1
    .pass1:
        mov er0, 0
    .loop3:
        st er0, [ea+]
        add er2, -2
        bnz .loop3
        mov r5, r5
        beq .exit
        st r4, [ea+]
    .exit:
        pop er4
        rt
    
    memset_n:
        push er4 // er4 param3
        add sp,2 // param3
        pop er4 // ...
        add sp,-4 // er4 param3 (restore stack)
        lea [er0]
    .loop2:
        st r2,[ea+]
        add er4,-1
        bne .loop2

        pop er4
        rt