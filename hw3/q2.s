.pos 0x100
	
	# tmp = 0
	ld $tmp, r0				# r0 = address of tmp
	ld $0, r1				# r1 = 0
	st r1, (r0)				# tmp = 0
	
	# tos = 0
	ld $0, r0				# r0 = 0
	ld $tos, r1				# r1 = address of tos
	st r0, (r1)				# tos = 0
	
	# s[tos] = a[0]
	ld $a, r0				# r0 = address of a
	ld (r0), r0				# r0 = a[0]
	ld $tos, r1				# r1 = address of tos
	ld (r1), r1				# r1 = tos
	ld $s, r2				# r2 = address of s
	st r0, (r2, r1, 4)		# s[tos] = a[0]
	
	# tos++
	ld $tos, r0				# r0 = address of tos
	ld (r0), r1				# r1 = tos
	inc r1					# r1 = tos++
	st r1, (r0)				# tos = tos++
	
	# s[tos] = a[1]
	ld $a, r0				# r0 = address of a
	ld 4(r0), r0			# r0 = a[1]
	ld $tos, r1				# r1 = address of tos
	ld (r1), r1				# r1 = tos
	ld $s, r2				# r2 = address of s
	st r0, (r2, r1, 4)		# s[tos] = a[1]
	
	# tos++
	ld $tos, r0				# r0 = address of tos
	ld (r0), r1				# r1 = tos
	inc r1					# r1 = tos++
	st r1, (r0)				# tos = tos++
	
	# s[tos] = a[2]
	ld $a, r0				# r0 = address of a
	ld 8(r0), r0			# r0 = a[2]
	ld $tos, r1				# r1 = address of tos
	ld (r1), r1				# r1 = tos
	ld $s, r2				# r2 = address of s
	st r0, (r2, r1, 4)		# s[tos] = a[2]
	
	# tos++
	ld $tos, r0				# r0 = address of tos
	ld (r0), r1				# r1 = tos
	inc r1					# r1 = tos++
	st r1, (r0)				# tos = tos++
	
	# tos--
	ld $tos, r0				# r0 = address of tos
	ld (r0), r1				# r1 = tos
	dec r1					# r1 = tos--
	st r1, (r0)				# tos = tos--
	
	# tmp = s[tos]
	ld $tos, r0				# r0 = address of tos
	ld (r0), r0				# r0 = tos
	ld $s, r1				# r1 = address of s
	ld (r1, r0, 4), r0		# r0 = s[tos]
	ld $tmp, r1				# r1 = address of tmp
	st r0, (r1)				# tmp = s[tos]
	
	# tos--
	ld $tos, r0				# r0 = address of tos
	ld (r0), r1				# r1 = tos
	dec r1					# r1 = tos--
	st r1, (r0)				# tos = tos--
	
	# tmp = s[tos]
	ld $tos, r0				# r0 = address of tos
	ld (r0), r0				# r0 = tos
	ld $s, r1				# r1 = address of s
	ld (r1, r0, 4), r0		# r0 = s[tos]
	ld $tmp, r1				# r1 = address of tmp
	ld (r1), r2				# r2 = tmp
	add r0, r2				# r2 = tmp + s[tos]
	st r2, (r1)				# tmp = tmp + s[tos]
	
	# tos--
	ld $tos, r0				# r0 = address of tos
	ld (r0), r1				# r1 = tos
	dec r1					# r1 = tos--
	st r1, (r0)				# tos = tos--
	
	# tmp = s[tos]
	ld $tos, r0				# r0 = address of tos
	ld (r0), r0				# r0 = tos
	ld $s, r1				# r1 = address of s
	ld (r1, r0, 4), r0		# r0 = s[tos]
	ld $tmp, r1				# r1 = address of tmp
	ld (r1), r2				# r2 = tmp
	add r0, r2				# r2 = tmp + s[tos]
	st r2, (r1)				# tmp = tmp + s[tos]

	halt

.pos 0x200
# Data area

a:		.long 1				# a[0]
		.long 2				# a[1]
		.long 3				# a[2]
s:		.long 4				# s[0]
		.long 5				# s[1]
		.long 6				# s[2]
		.long 7				# s[3]
		.long 8				# s[4]
tos:	.long 9				# tos
tmp:	.long 10			# tmp