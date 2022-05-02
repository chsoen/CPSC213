#include "int_element.h"
#include "refcount.h"
#include <stdio.h>

/* TODO: Implement all public int_element functions, including element interface functions.

You may add your own private functions here too. */

struct int_element_class {
    void (*int_element_print)(struct element *);
    int (*int_element_compare)(struct element *, struct element *);
    int (*int_element_get_value)(struct int_element *);
    int (*is_int_element)(struct element *);
};

struct int_element {
    struct int_element_class *class;
    int value;
};

void int_element_print(struct element *obj) {
    struct int_element *this = (struct int_element *) obj;
    printf("%d", this->value);
}

int int_element_compare(struct element *obj1, struct element *obj2) {
    if(is_int_element(obj1) && is_int_element(obj2)) {
        struct int_element *int1 = (struct int_element *) obj1;
        struct int_element *int2 = (struct int_element *) obj2;
        if(int1->value > int2->value) {
            return 1;
        } else if(int1->value == int2->value) {
            return 0;
        }
    }
    return -1;
}

struct int_element_class int_element_class = { &int_element_print, &int_element_compare, &int_element_get_value, &is_int_element };

int int_element_get_value(struct int_element *this) {
    return this->value;
}

int is_int_element(struct element *this) {
    return this->class == (void *) &int_element_class;
}

void int_element_finalizer(struct element *obj) {
    struct int_element *this = (struct int_element *) obj;
    rc_free_ref(this->class);
}

struct int_element *int_element_new(int value) {
    struct int_element *this = rc_malloc(sizeof(struct int_element), (rc_finalizer_t) int_element_finalizer);
    this->class = &int_element_class;
    this->value = value;
    return this;
}