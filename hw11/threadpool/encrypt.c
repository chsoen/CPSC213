#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <crypt.h>

int main(int argc, char *argv[]) {
  struct crypt_data data;
  puts(crypt_r(argv[1], argv[2], &data));
  return 0;
}
