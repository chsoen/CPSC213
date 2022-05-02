.pos 0x1000
code:

	# v0 = s.x[i]
	ld $i, r0			# r0 = address of i
	ld (r0), r0			# r0 = i
	ld $s, r1			# r1 = address of s
	shl $2, r0			# r0 = i * sizeof(int)
	add r0, r1			# r1 = address of s.x[i]
	ld (r1), r0			# r0 = s.x[i]
	ld $v0, r1			# r1 = address of v0
	st r0, (r1)			# v0 = s.x[i]
	
	# v1 = s.y[i]
	ld $i, r0			# r0 = address of i
	ld (r0), r0			# r0 = i
	ld $s, r1			# r1 = address of s
	ld 8(r1), r1		# r1 = s.y
	ld (r1, r0, 4), r1	# r1 = s.y[i]
	ld $v1, r0			# r0 = address of v1
	st r1, (r0)			# v1 = s.y[i]
	
	# v2 = s.z->x[i];
	ld $i, r0			# r0 = address of i
	ld (r0), r0			# r0 = i
	ld $s, r1			# r1 = address of s
	ld 12(r1), r1		# r1 = s.z
	shl $2, r0			# r0 = i * sizeof(int)
	add r0, r1			# r1 = &s.z->x[i]
	ld (r1), r1			# r1 = s.z->x[i]
	ld $v2, r0			# r0 = address of v2
	st r1, (r0)			# v2 = s.z->x[i]
	
	#v3 = s.z->z->y[i]
	ld $i, r0			# r0 = address of i
	ld (r0), r0			# r0 = i
	ld $s, r1			# r1 = address of s
	ld 12(r1), r1		# r1 = s.z
	ld 12(r1), r1		# r1 = s.z->z
	ld 8(r1), r1		# r1 = s.z->z->y
	ld (r1, r0, 4), r1	# r1 = s.z->z->y[i]
	ld $v3, r0			# r0 = address of v3
	st r1, (r0)			# v3 = s.z->z->y[i]
	
	
	halt

.pos 0x2000
static:
i:			.long 1
v0:			.long 2
v1:			.long 3
v2:			.long 4
v3:			.long 5
s:			.long 6			# x[0]
			.long 7			# x[1]
			.long s_y		# y
			.long s_z		# z

heap:
s_y:		.long 8			# s.y[0]
			.long 9			# s.y[1]
s_z:		.long 10		# s.z->x[0]
			.long 11		# s.z->x[1]
			.long 12		# s.z->y
			.long s_z_z		# s.z->z
s_z_z:		.long 13		# s.z->z->x[0]
			.long 14		# s.z->z->x[1]
			.long s_z_z_y	# s.z->z->y
			.long 15		# s.z->z->z
s_z_z_y:	.long 16		# s.z->z->y[0]
			.long 17		# s.z->z->y[1]