.pos 0x100
	ld $r, r0			# r0 <= &r
	ld (r0), r0			# r0 <= r
	ld $p, r1			# r1 <= &p
	ld (r1, r0, 4), r2	# r2 <= p[r]
	ld $3, r3			# r3 <= 3
	add r3, r0			# r0 <= r + 3
	ld (r1, r0, 4), r0	# r0 <= p[r+3]
	add r2, r0			# r0 <= p[r] + p[r+3]
	ld $x, r1			# r1 <= &x
	st r0, (r1)			# x <= p[r] + p[r+3]
	ld (r1), r1			# r1 <= x
	ld $0xf, r2			# r2 <= 0xf
	and r2, r1			# r1 <= x & 0xf
	ld $n, r0			# r0 <= &n
	st r1, (r0)			# n <= x & 0xf
.pos 0x1000
p:			.long 8	# p[0]
			.long 3 # p[1]
			.long 5 # p[2]
			.long 4 # p[3]
			.long 14 # p[4]
			.long 9 # p[5]
			.long 3 # p[6]
			.long 2 # p[7]
.pos 0x2000
r:			.long 1 # r
.pos 0x3000
n:			.long 0 # n
.pos 0x4000
x:			.long 0 # x