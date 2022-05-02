#include "str_element.h"
#include "refcount.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* TODO: Implement all public str_element functions, including element interface functions.

You may add your own private functions here too. */

struct str_element_class {
    void (*str_element_print)(struct element *);
    int (*str_element_compare)(struct element *, struct element *);
    char *(*str_element_get_value)(struct str_element *);
    int (*is_str_element)(struct element *);
};

struct str_element {
    struct str_element_class *class;
    char *value;
};

void str_element_print(struct element *obj) {
    struct str_element *this = (struct str_element *) obj;
    char *value = this->value;
    int index = 0;
    while (value[index] != 0) {
        printf("%c", value[index]);
        index++;
    }
}

int str_element_compare(struct element *obj1, struct element *obj2) {
    if (is_str_element(obj1) && is_str_element(obj2)) {
        char *value1 = ((struct str_element *) obj1)->value;
        char *value2 = ((struct str_element *) obj2)->value;
        return strcmp(value1, value2);
    }
    return 1;
}

struct str_element_class str_element_class = { &str_element_print, &str_element_compare, &str_element_get_value, &is_str_element };

char *str_element_get_value(struct str_element *this) {
    return this->value;
}

int is_str_element(struct element *this) {
    return this->class == (void *) &str_element_class;
}

void str_element_finalizer(struct element *obj) {
    struct str_element *this = (struct str_element *) obj;
    rc_free_ref(this->class);
    free(this->value);
}

struct str_element *str_element_new(char *value) {
    struct str_element *this = rc_malloc(sizeof(struct str_element), (rc_finalizer_t) str_element_finalizer);
    this->class = &str_element_class;
    this->value = strdup(value);
    return this;
}