#include <stdlib.h>
#include <stdio.h>

// YOU: Allocate these global variables, using these names
int  j, d;
int* r;
int  a[10];

int main (int argv, char** argc) {
  // Ignore this block of code
  if (argv != 11) {
    fprintf (stderr, "usage: a[0] ... a[9]\n");
    exit (EXIT_FAILURE);
  }
  for (int g=0; g<10; g++)
    a[g] = atol (argc[1 + g]);

  // YOU: Implement this code
  j  = a[6];
  j  = a[j];
  r  = &d;
  *r = 4;
  r  = &a[a[3]];
  *r = *r + a[9];

  // Ignore this block of code
  printf ("j=%d d=%d r=&a[%d] a={", j, d, r - a);
  for (int g=0; g<10; g++)
    printf("%d%s", a[g], g<9? ", ": "}\n");
  }
