        .ident "TYPE:"

        .ident "CODE:large"   
        .globl DebugOutputString
        .type DebugOutputString,@function
        .globl   memzero_n
        .type    memzero_n,@function
        .globl get_msb
        .type get_msb,@function
    DebugOutputString:
        lea [er0]
    loop7:
        l r0,r2:[ea+]
        st r0,6:0x0721
        bne loop7
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

    get_msb:
        mov r1,0
    .loop13:
        add r1,1
        srl r0,1
        mov r0,r0
        bne .loop13
        mov r0,r1
        add r0,-1
        rt