.pos 0x0
    ld   $0x1028, r5            # r5 = address of last word of stack
    ld   $0xfffffff4, r0        # r0 = -12
    add  r0, r5                 # sp -= 12 caller allocation
    ld   $0x200, r0             # r0 = & var0
    ld   0x0(r0), r0            # r0 = var0
    st   r0, 0x0(r5)            # i = var0
    ld   $0x204, r0             # r0 = & var1
    ld   0x0(r0), r0            # r0 = var1
    st   r0, 0x4(r5)            # num1 = var1
    ld   $0x208, r0             # r0 = & var2
    ld   0x0(r0), r0            # r0 = var2
    st   r0, 0x8(r5)            # num2 = var2
    gpc  $6, r6                 # r6 = pc + 6
    j    0x300                  # main();
    ld   $0x20c, r1             # r1 = &result
    st   r0, 0x0(r1)            # result = r0
    halt
.pos 0x200
    .long 16                    # var0
    .long 5                     # var1
    .long 8                     # var2
    .long 0x00000000            # result
.pos 0x300
    ld   0x0(r5), r0            # r0 = i
    ld   0x4(r5), r1            # r1 = num1
    ld   0x8(r5), r2            # r2 = num2
    ld   $0xfffffff6, r3        # r3 = -10
    add  r3, r0                 # r0 = i - 10
    mov  r0, r3                 # r3 = i - 10
    not  r3                     # r3 = !(i - 10)
    inc  r3                     # r3 = 10 - i
    bgt  r3, default                 # if (10 > i) goto default
    mov  r0, r3                 # r3 = i - 10
    ld   $0xfffffff8, r4        # r4 = -8
    add  r4, r3                 # r3 = i - 18
    bgt  r3, default                 # if (i > 18) goto default
    ld   $0x400, r3             # r3 = &jmptable
    ld   (r3, r0, 4), r3        # r3 = jmptable[i-10]
    j    (r3)                   # goto jmptable[i-10]
add:
    add  r1, r2                 # r2 = num1 + num2
    br   L7                     # goto L7
sub:
    not  r2                     # r2 = !num2
    inc  r2                     # r2 = -num2
    add  r1, r2                 # r2 = num1 - num2
    br   L7                     # goto L7
greater:
    not  r2                     # r2 = !num2
    inc  r2                     # r2 = -num2
    add  r1, r2                 # r2 = num1 - num2
    bgt  r2, L0                 # if (num1 > num2) goto L0
    ld   $0x0, r2               # r2 = 0
    br   L1                     # goto L1
L0:
    ld   $0x1, r2               # r2 = 1
L1:
    br   L7                     # goto L7
less:
    not  r1                     # r1 = !num1
    inc  r1                     # r1 = -num1
    add  r2, r1                 # r1 = num2 - num1
    bgt  r1, L2                 # if (num2 > num1) goto L2
    ld   $0x0, r2               # r2 = 0
    br   L3                     # goto L3
L2:
    ld   $0x1, r2               # r2 = 1
L3:
    br   L7                     # goto L7
equal:
    not  r2                     # r2 = !num2
    inc  r2                     # r2 = -num2
    add  r1, r2                 # r2 = num1 - num2
    beq  r2, L4                 # if (num1 == num2) goto L4
    ld   $0x0, r2               # r2 = 0
    br   L5                     # goto L5
L4:
    ld   $0x1, r2               # r2 = 1
L5:
    br   L7                     # goto L7
default:
    ld   $0x0, r2               # r2 = 0
    br   L7                     # goto L7
L7:
    mov  r2, r0                 # r0 = r2
    j    0x0(r6)                # return

.pos 0x400                      # jmptable
    .long add
    .long default
    .long sub
    .long default
    .long greater
    .long default
    .long less
    .long default
    .long equal
.pos 0x1000                     # stack
    .long 0x00000000
    .long 0x00000000
    .long 0x00000000
    .long 0x00000000
    .long 0x00000000
    .long 0x00000000
    .long 0x00000000
    .long 0x00000000
    .long 0x00000000
    .long 0x00000000