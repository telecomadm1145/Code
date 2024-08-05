;; Compile Options : 
;; Version Number  : Ver.1.30.0
;; File Name       : test.cpp

	.ident "TYPE:"
	.ident "CODE:large"
	.ident "DATA:far"
	.file  "test.cpp"

	.text
	.globl	main
	.type	main,@function
	.p2align	2
main :
	bl	_Z10reset_sfrsv
	mov	r0, 0x04
	st	r0, 0x0F037
	mov	r4, #0
	mov	r5, #8
	mov	r6, #0
	mov	r7, #-8
	mov	er8, #0
	mov	r10, #44
	mov	r11, #1

.LBB0_1:
	push	er4
	mov	er0, er6
	mov	er2, er8
	bl	memcpy_nn
	add	sp, 0x02
	mov	er0, er10
	bl	_Z5delayt
	mov	er0, er6
	mov	er2, er4
	bl	memzero_n
	mov	er0, er10
	bl	_Z5delayt
	b	.LBB0_1

.Lfunc_end0:
	.size	main, .Lfunc_end0-main


	.ident	"clang version 1.30"

