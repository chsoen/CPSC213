#include <stdio.h>

int y[] = {0, 0, 0, 0, 0, 0, 0, 0};

int x[] = {1, 2, 3, -1, -2, 0, 184, 340057058};

int f(int a0) {
    int count = 0;
    while(a0 != 0) {
        if((a0 & 0x80000000) != 0) {
            count++;
        }
        a0 = a0 << 1;
    }
    return count;
}

int main() {
    for(int i=8; i != 0; ) {
        i--;
        y[i] = f(x[i]);
    }
    for(int i=0; i<8; i++) {
        printf("%d\n", x[i]);
    }
    for(int i=0; i<8; i++) {
        printf("%d\n", y[i]);
    }
}