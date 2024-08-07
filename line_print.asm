        .ident "TYPE:"

        .ident "CODE:large"   
        .globl   lookup_bitmap
        .type    lookup_bitmap,@function
        .globl   render_copy
        .type    render_copy,@function

render_copy:
    push lr
    bl __regpushu8lw
    add sp, -0x04
    mov r8, r0
    mov r0, #0x00
    st r0, -3[fp]
    st r0, 0x91fc
    bal .l_01c
.l_018:
    b __regpopu8lw
.l_01c:
    mov r0, #0x00
    st r0,0xf037
    cmp r8, #0x00
    beq .l_02c
    mov r10, #0x54
    mov r11, #0xd6
    bal .l_030
.l_02c:
    mov r10, #0x54
    mov r11, #0xca
.l_030:
    mov r0, #0x00
    mov r1, #0xf8
    st er0, -2[fp]
    mov r9, #0x40
.l_038:
    mov bp, #0x00
.l_03a:
    mov er0, bp
    sllc r1, 2
    sll r0, 2
    mov er2, er0
    add er2, er10
    l er4, -2[fp]
    add er0, er4
    l er4, [er2]
    l er6, +2[er2]
    st er4, [er0]
    st er6, +2[er0]
    add bp, #0x01
    cmp r12, #0x06
    cmpc r13, #0x00
    blts .l_03a
    add er10, #0x18
    l er0, -2[fp]
    add er0, #0x20
    st er0, -2[fp]
    add r9, #-1
    mov r0, r9
    bne .l_038
    mov r0, #4
    st r0,0xf037
    l r0, -3[fp]
    bne .l_07a
    mov r0, #0x01
    st r0, -3[fp]
    bal .l_030
.l_07a:
    bal .l_018
