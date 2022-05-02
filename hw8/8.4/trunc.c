#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "list.h"

void print_str(element_t ev) {
    char *e = ev;
    printf("%s\n", e);
}

void print_str2(element_t ev) {
    char *e = ev;
    printf("%s\t", e);
}

void print_int(element_t ev) {
    int *e = ev;
    printf("%d\t", *e);
}

void create_ints(element_t *out, element_t in) {
    char *endp;
    *out = malloc(sizeof(int));
    int value = (int) strtol(in, &endp, 10);
    **((int **) out) = *endp == '\0' ? value : -1;
}

void create_strs(element_t *out, element_t integer, element_t string) {
    if(*(int *) integer == -1) {
        *(char **) out = (char *) string;
    } else {
        *out = NULL;
    }
}

int is_nonnegative(element_t integer) {
    return *(int *) integer >= 0;
}

int is_nonnull(element_t string) {
    return (char *) string != NULL;
}

void truncate(element_t *out, element_t integer, element_t string) {
    char *truncated = malloc(*(int *) integer + 1);
    strncpy(truncated, (char *) string, *(int *) integer);
    truncated[*(int *) integer] = '\0';
    *((char **) out) = truncated;
}

void concatenate(element_t *outp, element_t out, element_t in) {
    if((char *) out == NULL) {
        char *temp = malloc(strlen((char *) in) + 1);
        strncpy(temp, (char *) in, strlen((char *) in));
        temp[strlen((char *) in)] = '\0';
        *((char **) outp) = temp;
        return;
    }
    char *temp = malloc(strlen(out) + strlen(in) + 2);
    strncpy(temp, out, strlen(out));
    temp[strlen(out)] = ' ';
    temp[strlen(out)+1] = '\0';
    strcat(temp, (char *) in);
    free(out);
    *((char **) outp) = temp;
}

void max(element_t *outp, element_t out, element_t in) {
    if(*(int *) out < *(int *) in) {
        **(int **) outp = *(int *) in;
    }
}

int main(int argc, char **argv) {
    struct list* l1 = list_create();
    for(int i=1; i<argc; i++) {
        list_append(l1, (element_t) argv[i]);
    }

    struct list* l2 = list_create();
    list_map1(create_ints, l2, l1);

    struct list* l3 = list_create();
    list_map2(create_strs, l3, l2, l1);

    struct list* l4 = list_create();
    list_filter(is_nonnegative, l4, l2);

    struct list* l5 = list_create();
    list_filter(is_nonnull, l5, l3);

    struct list* l6 = list_create();
    list_map2(truncate, l6, l4, l5);

    list_foreach(print_str, l6);

    char *result = NULL;
    list_foldl(concatenate, (element_t) &result, l6);
    if(result != NULL) {
        printf("%s\n", result);
    }

    int *maximum = malloc(sizeof(int));
    *maximum = 0;
    list_foldl(max, (element_t *) &maximum, l4);
    printf("%d\n", *maximum);
    
    list_destroy(l1);
    list_foreach(free, l2);
    list_destroy(l2);
    list_destroy(l3);
    list_destroy(l4);
    list_destroy(l5);
    list_foreach(free, l6);
    list_destroy(l6);
    free(result);
    result = NULL;
    free(maximum);
    maximum = NULL;
}
