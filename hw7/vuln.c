#include <unistd.h>

struct string {
  int size;
  char *string;
};

char _str1[] = "Welcome! Please enter a name:\n";
struct string str1 = { .size = 30, .string = _str1};

char _str2[] = "Good luck, ";
struct string str2 = { .size = 11, .string = _str2};

char _str3[] = "The secret phrase is \"squeamish ossifrage\"\n";
struct string str3 = { .size = 43, .string = _str3};

void print(struct string str) {
    write(1, str.string, str.size);
}

int main(int arc, char *argv[]) {
    print(str1);
    char buf[128];
    int size = read(0, buf, 256);
    print(str2);
    write(1, buf, size);
}

void proof() {
    print(str3);
}