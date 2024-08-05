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
	mov	fp, sp
	add	sp, #-32
	mov	r8, #SEG .L__const.main.test
	lea	OFFSET	.L__const.main.test+22
	l	r0, r8:[ea+]
	l	r1, DSR:[ea+]
	l	r2, DSR:[ea+]
	l	r3, DSR:[ea+]
	l	r4, DSR:[ea+]
	l	r5, DSR:[ea+]
	l	r6, DSR:[ea+]
	l	r7, DSR:[ea]
	lea	0x0fff8[fp]
	st	qr0, [ea]
	mov	r8, #SEG .L__const.main.test
	lea	OFFSET	.L__const.main.test+16
	l	r0, r8:[ea+]
	l	r1, DSR:[ea+]
	l	r2, DSR:[ea+]
	l	r3, DSR:[ea+]
	l	r4, DSR:[ea+]
	l	r5, DSR:[ea+]
	l	r6, DSR:[ea+]
	l	r7, DSR:[ea]
	lea	0x0fff2[fp]
	st	qr0, [ea]
	mov	r8, #SEG .L__const.main.test
	lea	OFFSET	.L__const.main.test+8
	l	r0, r8:[ea+]
	l	r1, DSR:[ea+]
	l	r2, DSR:[ea+]
	l	r3, DSR:[ea+]
	l	r4, DSR:[ea+]
	l	r5, DSR:[ea+]
	l	r6, DSR:[ea+]
	l	r7, DSR:[ea]
	lea	0x0ffea[fp]
	st	qr0, [ea]
	mov	r8, #SEG .L__const.main.test
	lea	OFFSET	.L__const.main.test
	l	r0, r8:[ea+]
	l	r1, DSR:[ea+]
	l	r2, DSR:[ea+]
	l	r3, DSR:[ea+]
	l	r4, DSR:[ea+]
	l	r5, DSR:[ea+]
	l	r6, DSR:[ea+]
	l	r7, DSR:[ea]
	lea	0x0ffe2[fp]
	st	qr0, [ea]
	bl	_Z10reset_sfrsv
	mov	r0, #-1
	mov	r1, #-1
	st	er0, 0x0F020
	mov	er0, #49
	mov	r2, #0
	st	er0, -18[fp]
	st	r2, -16[fp]
	mov	r0, #55
	mov	r1, #50
	st	er0, -20[fp]
	mov	r0, 0x0A
	st	r0, -17[fp]
	mov	r0, 0x037
	mov	r1, 0x032
	mov	r2, 0x031
	mov	r3, 0x030
	st	er0, -24[fp]
	st	er2, -22[fp]
	mov	er0, fp
	add	er0, #-30
	mov	r2, 0x00
	bl	DebugOutputString
	mov	r4, #-24
	mov	r5, #3

.LBB0_1:
	mov	er0, er4
	bl	_Z5delayt
	b	.LBB0_1

.Lfunc_end0:
	.size	main, .Lfunc_end0-main

	.type	.L__const.main.test,@object
	.section	.rodata,"a",@progbits
.L__const.main.test:
	.byte	67
	.byte	111
	.byte	117
	.byte	110
	.byte	116
	.byte	58
	.zero	24
	.size	.L__const.main.test, 30


	.ident	"clang version 1.30"

