    .sect ".boot"
	.nocmp

    .global _reset
	.global	_c_int00

_reset:
	b	_c_int00
	nop
	nop
	nop
	nop
	nop
	nop
	nop
_nmi:
	b	_nmi
	nop
	nop
	nop
	nop
	nop
	nop
	nop
_resv1:
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
_resv2:
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
_int4:
	b	_int4
	nop
	nop
	nop
	nop
	nop
	nop
	nop
_int5:
	b	_int5
	nop
	nop
	nop
	nop
	nop
	nop
	nop
_int6:
	b	_int6
	nop
	nop
	nop
	nop
	nop
	nop
	nop
_int7:
	b	_int7
	nop
	nop
	nop
	nop
	nop
	nop
	nop
_int8:
	b	_int8
	nop
	nop
	nop
	nop
	nop
	nop
	nop
_int9:
	b	_int9
	nop
	nop
	nop
	nop
	nop
	nop
	nop
_int10:
	b	_int10
	nop
	nop
	nop
	nop
	nop
	nop
	nop
_int11:
	b	_int11
	nop
	nop
	nop
	nop
	nop
	nop
	nop
_int12:
	b	_int12
	nop
	nop
	nop
	nop
	nop
	nop
	nop
_int13:
	b	_int13
	nop
	nop
	nop
	nop
	nop
	nop
	nop
_int14:
	b	_int14
	nop
	nop
	nop
	nop
	nop
	nop
	nop
_int15:
	b	_int15
	nop
	nop
	nop
	nop
	nop
	nop
	nop
