#include <stdio.h>

int var0 = 10;
int var1 = 5;
int var2 = 4;
int result = 0;

int q2(int i, int a, int b) {
    static const void* jumpTable[9] = {&&add, &&DEFAULT,
                                        &&sub, &&DEFAULT,
                                        &&greater, &&DEFAULT,
                                        &&less, &&DEFAULT,
                                        &&equal};
    
    if (i < 10 || i > 18) goto DEFAULT;
    goto *jumpTable[i-10];
add:
    result = a + b;
    goto L7;
sub:
    result = a - b;
    goto L7;
greater:
    result = a > b;
    goto L7;
less:
    result = a < b;
    goto L7;
equal:
    result = a == b;
    goto L7;
DEFAULT:
    result = 0;
    goto L7;
L7:
    return result;
}

int main() {
    printf("%d\n", q2(var0, var1, var2));
}