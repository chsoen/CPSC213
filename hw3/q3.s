.pos 0x100
		
	# a = 3
	ld $3, r0				# r0 = 3
	ld $a, r1				# r1 = address of a
	st r0, (r1)				# a = 3
	
	# p = &a
	ld $a, r0				# r0 = address of a
	ld $p, r1				# r1 = address of p
	st r0, (r1)				# p = &a
	
	# *p = *p - 1
	ld $p, r0				# r0 = address of p
	ld (r0), r0				# r0 = p = address of a
	ld (r0), r1				# r1 = a
	dec r1					# r1 = *p - 1 = a - 1
	st r1, (r0)				# *p = *p - 1
	
	# p = &b[0]
	ld $b, r0				# address of b
	ld $p, r1				# address of p
	st r0, (r1)				# p = &b[0]
	
	# p++
	ld $p, r0				# r0 = address of p
	ld (r0), r1				# r1 = p = b[0]
	ld $4, r2				# r2 = 4 = sizeof(int)
	add r2, r1				# r1 = p + sizeof(int)
	st r1, (r0)				# p++
	
	# p[a] = b[a]
	ld $a, r0				# r0 = address of a
	ld (r0), r0				# r0 = a
	ld $b, r1				# r1 = address of b
	ld (r1, r0, 4), r1		# r1 = b[a]
	ld $p, r2				# r2 = address of p
	ld (r2), r2				# r2 = p = b[1]
	st r1, (r2, r0, 4)		# p[a] = b[a]
	
	# *(p+3) = b[0]
	ld $b, r0				# r0 = address of b
	ld (r0), r0				# r0 = b[0]
	ld $p, r1				# r1 = address of p
	ld (r1), r1				# r1 = p = b[1]
	ld $3, r2				# r2 = 3
	shl $2, r2				# r2 = sizeof(int) * 3
	add r2, r1				# r1 = p+3
	st r0, (r1)				# *(p+3) = b[0]

	halt

.pos 0x200
# Data area

a:		.long 1				# a
p:		.long 2				# p
b:		.long 3				# b[0]
		.long 4				# b[1]
		.long 5				# b[2]
		.long 6				# b[3]
		.long 7				# b[4]