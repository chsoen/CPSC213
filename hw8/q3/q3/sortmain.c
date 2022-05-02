#include <stdio.h>
#include <stdlib.h>

#include "int_element.h"
#include "str_element.h"
#include "element.h"
#include "refcount.h"

/* If the string is numeric, return an int_element. Otherwise return a str_element. */
struct element *parse_string(char *str) {
  char *endp;
  /* strtol returns a pointer to the first non-numeric character in endp.
     If it gets to the end of the string, that character will be the null terminator. */
  int value = strtol(str, &endp, 10);
  if(str[0] != '\0' && endp[0] == '\0') {
    /* String was non-empty and strtol conversion succeeded - integer */
    return (struct element *)int_element_new(value);
  } else {
    return (struct element *)str_element_new(str);
  }
}

int compare(const void *obj1, const void *obj2) {
  struct element *elem1 = *(struct element **) obj1;
  struct element *elem2 = *(struct element **) obj2;
  return elem1->class->compare(elem1, elem2);
}

int main(int argc, char **argv) {
  /* TODO: Read elements into a new array using parse_string */
  struct element **array = malloc((argc-1)*sizeof(void *));
  for (int i=0; i<argc-1; i++) {
    array[i] = parse_string(argv[i+1]);
  }

  /* TODO: Sort elements with qsort */
  qsort(array, argc-1, sizeof(void*), compare);
  printf("Sorted: ");

  /* TODO: Print elements, separated by a space */
  for (int i=0; i<argc-1; i++) {
    array[i]->class->print(array[i]);
    printf(" ");
    rc_free_ref(array[i]);
  }
  printf("\n");
  free(array);
}
