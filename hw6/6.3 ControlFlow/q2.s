.pos 0x100

ld $0, r0                   # r0 = i';
ld $n, r1
ld (r1), r1                 # r1 = n
not r1
inc r1                      # r1 = -n;
ld $a, r2                   # r2 = &a;
ld $b, r3                   # r3 = &b;
ld $c, r4
ld (r4), r4                 # r4 = c';

loop:   mov r0, r5          # r5 = i'
        add r1, r5          # r5 = i'-n
        bgt r5, end
        beq r5, end         # if (i' >= n) goto end;
        ld (r3, r0, 4), r6  # r6 = b[i]
        ld (r2, r0, 4), r7  # r7 = a[i]
        not r7
        inc r7              # r7 = -a[i]
        add r6, r7          # r7 = b[i] - a[i]
        bgt r7, else
        beq r7, else        # if (a[i] <= b[i]) goto else;
        inc r4              # c'++;
else:   inc r0              # i'++;
        br loop             # goto loop;
end:    ld $i, r1           # r0 = &i
        st r0, (r1)         # r1 = i
        ld $c, r2           # r0 = &c
        st r4, (r2)         # c = c'

halt

.pos 0x200
i:		.long -1    # i
n:		.long 5     # n
a:		.long 10    # a[0]
		.long 20    # a[1]
		.long 30    # a[2]
		.long 40    # a[3]
		.long 50    # a[4]
b:		.long 11    # b[0]
		.long 20    # b[1]
		.long 28    # b[2]
		.long 44    # b[3]
		.long 48    # b[4]
c:		.long 0     # c