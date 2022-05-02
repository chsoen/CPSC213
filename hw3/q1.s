.pos 0x100
	# j = a[6]
	ld $6, r0			# r0 = 6
	ld $a, r1			# r1 = address of a
	ld (r1, r0, 4), r0	# r0 = a[6]
	ld $j, r2			# r2 = address of j
	st r0, (r2)			# j = a[6]
	
	# j = a[j]
	ld $j, r0			# r0 = address of j
	ld (r0), r1			# r1 = j
	ld $a, r2			# r2 = address of a
	ld (r2, r1, 4), r1	# r1 = a[j]
	st r1, (r0)			# j = a[j]
	
	# r = &d
	ld $r, r0			# r0 = address of r
	ld $d, r1			# r1 = address of d
	st r1, (r0)			# r = &d
	
	# *r = 4
	ld $4, r0			# r0 = 4
	ld $r, r1			# r1 = address of r
	ld (r1), r1			# r1 = *r = address of d
	st r0, (r1)			# *r = 4
	
	# r = &a[a[3]]
	ld $3, r0			# r0 = 3
	ld $a, r1			# r1 = address of a
	ld (r1, r0, 4), r0	# r0 = a[3]
	shl $2, r0			# r0 = sizeof(int) * a[3]
	add r1, r0			# r0 = &a[a[3]]
	ld $r, r1			# r1 = address of r
	st r0, (r1)			# r = &a[a[3]]
	
	# *r = *r + a[9]
	ld $9, r0			# r0 = 9
	ld $a, r1			# r1 = address of a
	ld (r1, r0, 4), r1	# r1 = a[9]
	ld $r, r0			# r0 = address of r
	ld (r0), r0			# r0 = r = address of a[a[3]]
	ld (r0), r2			# r0 = *r = a[a[3]]
	add r2, r1			# r2 = *r + a[9]
	st r1, (r0)			# *r = *r + a[9]
	
	halt
	
.pos 0x200
# Data area

j:	.long 0		# j
d:	.long 0		# d
r:	.long 0		# r
a:	.long 1		# a[0]
	.long 2		# a[1]
	.long 3		# a[2]
	.long 4		# a[3]
	.long 5		# a[4]
	.long 6		# a[5]
	.long 7		# a[6]
	.long 8		# a[7]
	.long 9		# a[8]
	.long 10	# a[9]