.pos 0x100
	ld $0x1000, r0		# r0 = pointer of array
	ld $5, r1			# r1 = 5
	ld (r0, r1, 4), r2	# r2 = array[5]
	ld $4, r3			# r3 = 4
	ld (r0, r3, 4), r4	# r4 = array[4]
	st r4, (r0, r1, 4)	# array[5] = array[4]
	st r2, (r0, r3, 4)	# array[4] = t
	halt				# halt
.pos 0x1000
array:		.long 0	# array[0] 
			.long 1	# array[1]
			.long 2	# array[2]
			.long 3	# array[3]
			.long 4	# array[4]
			.long 5	# array[5]
			.long 6	# array[6]
			.long 7	# array[7]