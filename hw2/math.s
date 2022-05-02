.pos 0x100
	ld $e, r0		# r0 <= &e
	ld (r0), r0		# r0 <= e
	mov r0, r1		# r1 <= e
	inc r1			# r1 = e + 1
	inca r1			# r1 = (e + 1) + 4
	shl $6, r1		# r1 = ((e + 1) + 4) << 6
	and r0, r1		# r1 = (((e + 1) + 4) << 6) & e
	shr $3, r1		# r1 = ((((e + 1) + 4) << 6) & e) >> 4
	ld $b, r0		# r0 <= &b
	st r1, (r0)		# b <= r1
	halt
.pos 0x1000
b:			.long 0xffffffff	# b
.pos 0x2000
e:			.long 0xfedcba98 # e