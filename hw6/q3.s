
# Compute averages
ld $s, r0                   # r0 = &s
ld (r0), r0                 # r0 = s'

ld $n, r1                   # r1 = &n
ld (r1), r1                 # r1 = n
not r1
inc r1                      # r1 = -n

ld $0, r2                   # r2 = i'

avg_loop:
    mov r2, r3          # r3 = i'
    add r1, r3          # r3 = i' - n
    bgt r3, avg_end
    beq r3, avg_end     # if (i' >= n) goto end;
    ld $0, r3           # r3 = sum = 0
    ld 4(r0), r4        # r4 = s[i].grade[0]
    add r4, r3          # r3 = sum + s[i].grade[0]
    ld 8(r0), r4        # r4 = s[i].grade[1]
    add r4, r3          # r3 = sum + s[i].grade[1]
    ld 12(r0), r4       # r4 = s[i].grade[2]
    add r4, r3          # r3 = sum + s[i].grade[2]
    ld 16(r0), r4       # r4 = s[i].grade[3]
    add r4, r3          # r3 = sum + s[i].grade[3]
    shr $2, r3          # r3 = sum / 4
    st r3, 20(r0)       # s[i].average = sum / 4;
    inc r2              # i'++;
    ld $24, r3          # r3 = 24
    add r3, r0          # s' + 1;
    br avg_loop          # goto avg_loop;
avg_end:

# Bubble Sort
ld $n, r0               # r0 = &n
ld (r0), r0             # r0 = n
dec r0                  # r0 = i' = n-1

sort_outer:
    mov r0, r1          # r1 = i'
    not r1
    inc r1              # r1 = -i'
    beq r1, sort_outend # if (i' == 0) goto sort_outend;
    
    ld $0, r1           # r1 = j' = 0
sort_inner:
    mov r0, r2          # r2 = i'
    not r2
    inc r2              # r2 = -i'
    mov r1, r3          # r3 = j'
    add r2, r3          # r3 = j' - i'
    bgt r3, sort_inend
    beq r3, sort_inend  # if (j' > i') goto sort_inend;
    
    mov r1, r2          # r2 = j'
    shl $4, r2          # r2 = j' * 16
    mov r1, r3          # r3 = j'
    shl $3, r3          # r3 = j' * 8
    add r3, r2          # r2 = j' * 24
    ld $s, r3           # r3 = &s
    ld (r3), r3         # r3 = s
    add r3, r2          # r2 = s[j]
    
    ld 20(r2), r3       # r3 = s[j].average
    ld 44(r2), r2       # r2 = s[j+1].average
    not r3
    inc r3              # r3 = -s[j].average
    add r3, r2          # r2 = s[j+1].average - s[j].average
    bgt r2, else
    beq r2, else        # if (s[j+1].average >= s[j].average) goto else;
    
    mov r1, r4          # r4 = arg0 = j'
    gpc $6, r6          # ra = pc + 6
    j swap              # swap(j')
else:
    inc r1              # j'++;
    br sort_inner       # goto sort_inner;
sort_inend:
    dec r0              # i'--;
    br sort_outer       # goto sort_outer;
sort_outend:

ld $n, r0               # r0 = &n
ld (r0), r0             # r0 = n
shr $1, r0              # r0 = n/2

mov r0, r1              # r1 = median
shl $4, r1              # r1 = median * 16
shl $3, r0              # r0 = median * 8
add r1, r0              # r0 = median * 24
ld $s, r2               # r2 = &s
ld (r2), r2             # r2 = s
add r2, r0              # r0 = s[median]
ld (r0), r0             # r0 = s[median].sid

ld $m, r1               # r1 = &m
st r0, (r1)             # m = s[median].sid

halt

.pos 0x3000
# Swap function
swap:
    mov r4, r7          # r7 = j
    shl $4, r7          # r7 = j * 16
    shl $3, r4          # r4 = j * 8
    add r7, r4          # r4 = j * 24
    ld $s, r2           # r2 = &s
    ld (r2), r2         # r2 = s
    add r4, r2          # r2 = s[j]
    ld $6, r3           # r3 = i' = 6
swap_loop:
    beq r3, swap_end    # if (i' == 0) goto swap_end;
    ld (r2), r4         # r4 = s[j]
    ld 24(r2), r7       # r7 = s[j+1]
    st r7, (r2)         # s[j] = s[j+1]
    st r4, 24(r2)       # s[j+1] = s[j]
    dec r3              # i'--;
    inca r2             # iterate through struct members
    br swap_loop        # goto swap_loop;
swap_end:
    j (r6)              # return

.pos 0x2000
n:		.long 7     # size of s
m:		.long 0     # median student's id
s:		.long base  # students array
base:	.long 0110  # student ID
		.long 50    # grade 0
		.long 50    # grade 1
		.long 50    # grade 2
		.long 50    # grade 3
		.long 0     # computed average
        .long 6666  # student ID
		.long 20    # grade 0
		.long 20    # grade 1
		.long 20    # grade 2
		.long 20    # grade 3
		.long 0     # computed average
        .long 1234  # student ID
		.long 80    # grade 0
		.long 80    # grade 1
		.long 80    # grade 2
		.long 80    # grade 3
		.long 0     # computed average
        .long 4444  # student ID
		.long 70    # grade 0
		.long 70    # grade 1
		.long 70    # grade 2
		.long 70    # grade 3
		.long 0     # computed average
        .long 5555  # student ID
		.long 10    # grade 0
		.long 10    # grade 1
		.long 10    # grade 2
		.long 10    # grade 3
		.long 0     # computed average
        .long 1  # student ID
		.long 20    # grade 0
		.long 20    # grade 1
		.long 20    # grade 2
		.long 20    # grade 3
		.long 0     # computed average
        .long 7777  # student ID
		.long 40    # grade 0
		.long 40    # grade 1
		.long 40    # grade 2
		.long 40    # grade 3
		.long 0     # computed average