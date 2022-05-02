.pos 0x100
    ld  $x, r0
	ld $t, r2
	ld (r2), r3
	ld (r0, r3, 4), r4
	add r3, r4
	ld $b, r1
	st r4, (r1, r3, 4)
.pos 0x1000
x:	.long 2
	.long 0xff
	.long 0xff
	.long 0xff
	.long 0xff
.pos 0x2000
t:	.long 2
.pos 0x3000
b:	.long 0
	.long 1
	.long 2
	.long 3
	.long 4
	.long 5