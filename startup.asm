;***************************************************************
;       ML622502 start up assembly source file
;       for CCU8 version 1.xx/2.xx/3.xx
;       SMALL CODE MODEL
;       ROMWINDOW  00000H-0CFFFH
;       Version 1.10
;       Last Edition  Apr 18, 2023
;       Copyright (C) 2023 LAPIS Technology Co., Ltd.
;       U8/U16 Device Information Files V2.01
;***************************************************************

        .ident "TYPE:"

        .ident "CODE:large"   



        .section .text.isr_vector,"ax",@progbits
        .short  _$$SP              ;; dw offset _$$SP @ 0:0h
        .short  _$$start_up        ;; dw start_up     @ 0:2h

        .short  _$$brk_reset       ;; dw brk_reset    @ 0:4h

        .short   _$$brk_reset



        .section .text0.start_up,"ax",@progbits
        .globl   _$$start_up
        .type    _$$start_up,@function
_$$start_up:
        bal      _$begin

_$$brk_reset:
_$$error:
_$$bal_00:
        bal     _$$bal_00

_$begin:
        b       main