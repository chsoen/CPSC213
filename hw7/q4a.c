#include <stdio.h>

int base[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int *arr = base;

void fn(int a0, int a1) {
    arr[a1] += a0;
}

int main() {
    int l0 = 1;
    int l1 = 2;
    fn(3, 4);
    fn(l0, l1);
    for (int i=0; i<10; i++) {
        printf("%d\n", arr[i]);
    }
}