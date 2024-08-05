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
    l r0, 0x9208
    beq .l_01c
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

f_27432:
    push xr4
    push er8
    mov er8, #0x00
    mov r6, r0
    mul er6, r2
    mov r4, r0
    mul er4, r3
    add r7, r4
    addc r8, r5
    mov r4, r1
    mul er4, r2
    add r7, r4
    addc r8, r5
    addc r9, #0x00
    mov r2, r1
    mul er2, r3
    add er2, er8
    mov er0, er6
    pop er8
    pop xr4
    rt

lookup_bitmap:
	push lr
	bl __regpushu8lw
	add sp, -0x12
	mov er8, er0
	mov r5, r0
	mov er6, er2
	mov r0, #0xd
	mov r10, r0
	mov r0, #0xa
	mov r11, r0
	mov er0, #-0x40
	st er0, -0x10[fp]
	cmp r9, #0x0
	bne .l_022
	b .f_28890
.l_022:
	mov r0, r9
	mov r1, #0x0
	and r0, #0xf
	and r1, #0x0
	sllc r1, 2
	sll r0, 2
	l er2, +0x50a0[er0]
	l r0, +0x50a2[er0]
	st er2, -0xe[fp]
	st r0, -0xc[fp]
	cmp r9, #0xf1
	beq .l_042
	cmp r9, #0xf2
	bne .l_04e
.l_042:
	mov r0, #0xc
	mov r10, r0
	mov r0, #0xb
	mov r11, r0
	mov er0, #-0x20
	st er0, -0x10[fp]
.l_04e:
	add r5, #-0x10
	mov r2, r5
	mov r0, r11
	mul er2, r0
	mov er0, er2
	srlc r0, 4
	sra r1, 4
	mov bp, er0
	sllc r1, 4
	sll r0, 4
	sub r2, r0
	subc r3, r1
	mov er4, er2
	mov r0, r10
	mov r1, #0x0
	mov er2, bp
	bl f_27432
	add er0, er0
	l er2, -0xe[fp]
	add er0, er2
	l r2, -0xc[fp]
	st er0, -0x6[fp]
	st r2, -0x4[fp]
	mov er0, #0x0
	st er0, -0x8[fp]
	bal .l_0e6
.l_084:
	l bp, -0x6[fp]
	l r0, -0x4[fp]
	l er0, r0:[bp]
	mov er2, er4
	beq .l_098
.l_090:
	sllc r1, 1
	sll r0, 1
	add er2, #-1
	bne .l_090
.l_098:
	st er0, -0x12[fp]
	mov er4, er4
	beq .l_0ca
	mov r0, r10
	mov r1, #0x0
	add er0, er0
	l er2, -0x6[fp]
	add er0, er2
	l r2, -0x4[fp]
	mov bp, er0
	mov er0, #0x10
	sub r0, r4
	subc r1, r5
	l er2, r2:[bp]
	mov er0, er0
	beq .l_0c2
.l_0ba:
	srlc r2, 1
	srl r3, 1
	add er0, #-1
	bne .l_0ba
.l_0c2:
	l er0, -0x12[fp]
	or r0, r2
	or r1, r3
	st er0, -0x12[fp]
.l_0ca:
	l er2, -0x10[fp]
	and r0, r2
	and r1, r3
	st er0, -0x12[fp]
	srlc r0, 1
	srl r1, 1
	st er0, [er6]
	l er0, -0x8[fp]
	add er0, #0x1
	st er0, -0x8[fp]
	l er0, -0x6[fp]
	add er0, #0x2
	st er0, -0x6[fp]
	add er6, #0x2
.l_0e6:
	mov r0, r10
	mov r1, #0x0
	l er2, -0x8[fp]
	cmp er2, er0
	blts .l_084
	bal .l_0fe
.l_0f2:
	mov er0, #0x0
	st er0, [er6]
	l er0, -0x8[fp]
	add er0, #0x1
	st er0, -0x8[fp]
	add er6, #0x2
.l_0fe:
	l er0, -0x8[fp]
	cmp r0, #0x10
	cmpc r1, #0x0
	blts .l_0f2
.l_106:
	b __regpopu8lw

.f_28890:
	mov r0, #0xa0
	mov r1, #0x48
	mov r2, #0x4
	st er0, -0xe[fp]
	st r2, -0xc[fp]
	cmp r8, #0x7c
	beq .al_012
	b .l_04e
.al_012:
	mov er0, #0x0
	bal .al_024
.al_016:
	add er0, er0
	add er0, er6
	mov r2, #0x0
	mov r3, #0xe0
	st er2, [er0]
	l er0, -0x8[fp]
	add er0, #0x1
.al_024:
	st er0, -0x8[fp]
	cmp r0, #0xd
	cmpc r1, #0x0
	blts .al_016
	bal .l_106