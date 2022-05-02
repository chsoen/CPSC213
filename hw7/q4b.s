.pos 0x100
start:
    ld   $sb, r5         # sp = address of last word of stack
    inca r5              # sp = address of word after stack
    gpc  $6, r6          # r6 = pc + 6
    j    main            # call main
    halt

f:
    deca r5              # sp -= 4 callee allocation
    ld   $0, r0          # r0 = count = 0;
    ld   4(r5), r1       # r1 = a0
    ld   $0x80000000, r2 # r2 = 0x80000000
f_loop:
    beq  r1, f_end       # if (a0 == 0) goto f_end
    mov  r1, r3          # r3 = a0
    and  r2, r3          # r3 = a0 & 0x80000000
    beq  r3, f_if1       # if (a0 & 0x80000000 == 0) goto f_if1
    inc  r0              # r0 += 1
f_if1:
    shl  $1, r1          # r1 = a0 << 1
    br   f_loop          # goto f_loop
f_end:
    inca r5              # sp += 4 callee deallocation
    j    (r6)            # return

main:
    deca r5              # sp -= 4
    deca r5              # sp -= 4 callee allocation
    st   r6, 4(r5)       # store ra
    ld   $8, r4          # r4 = 8
main_loop:
    beq  r4, main_end    # if (i' == 0) goto main_end
    dec  r4              # r4 -= 1
    ld   $x, r0          # r0 = &x
    ld   (r0, r4, 4), r0 # r0 = x[i]
    deca r5              # sp -= 4 caller allocation
    st   r0, (r5)        # a0 = x[i]
    gpc  $6, r6          # r6 = pc + 6
    j    f               # f(a0);
    inca r5              # sp += 4 caller deallocation
    ld   $y, r1          # r1 = &y
    st   r0, (r1, r4, 4) # y[i] = f(x[i]);
    br   main_loop       # goto main_loop
main_end:
    ld   4(r5), r6       # load ra
    inca r5              # sp -= 4
    inca r5              # sp -= 4 callee deallocation
    j    (r6)            # return

.pos 0x2000
x:
    .long 1
    .long 2
    .long 3
    .long -1
    .long -2
    .long 0
    .long 184
    .long 340057058

y:
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0

.pos 0x8000
# These are here so you can see (some of) the stack contents.
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
sb: .long 0

