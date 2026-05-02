#include "person.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Person* person_create(const char* full_name, int age, int id) {
    Person* p = malloc(sizeof(Person));
    if (!p) return NULL;
    if (full_name) {
        strncpy(p->full_name, full_name, sizeof(p->full_name) - 1);
        p->full_name[sizeof(p->full_name) - 1] = '\0';
    } else {
        p->full_name[0] = '\0';
    }
    p->age = age;
    p->id = id;
    return p;
}

void person_destroy(Person* p) {
    free(p);
}

void* person_clone(const void* src) {
    if (!src) return NULL;
    const Person* s = (const Person*)src;
    Person* p = malloc(sizeof(Person));
    if (!p) return NULL;
    memcpy(p, s, sizeof(Person));
    return p;
}

int person_compare(const void* a, const void* b) {
    if (!a || !b) return 0;
    const Person* pa = (const Person*)a;
    const Person* pb = (const Person*)b;
    int cmp = strcmp(pa->full_name, pb->full_name);
    return cmp ? cmp : (pa->id - pb->id);
}

void person_print(const void* p) {
    if (!p) return;
    const Person* x = (const Person*)p;
    printf("[Person] ID: %d, Name: %s, Age: %d\n", x->id, x->full_name, x->age);
}