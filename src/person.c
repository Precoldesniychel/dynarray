#include "person.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Person* person_create(const char* full_name, int age, int id) {
    Person* p = malloc(sizeof(Person));
    if (!p) return NULL;
#ifdef _MSC_VER
    p->full_name = full_name ? _strdup(full_name) : NULL;
#else
    p->full_name = full_name ? strdup(full_name) : NULL;
#endif
    p->age = age;
    p->id = id;
    return p;
}

void person_destroy(Person* p) {
    if (!p) return;
    free(p->full_name);
    free(p);
}

void* person_clone(const void* src) {
    if (!src) return NULL;
    const Person* s = (const Person*)src;
    return person_create(s->full_name, s->age, s->id);
}

int person_compare(const void* a, const void* b) {
    if (!a || !b) return 0;
    const Person* pa = (const Person*)a;
    const Person* pb = (const Person*)b;
    int cmp = strcmp(pa->full_name ? pa->full_name : "", pb->full_name ? pb->full_name : "");
    return cmp ? cmp : (pa->id - pb->id);
}

void person_print(const void* p) {
    if (!p) return;
    const Person* x = (const Person*)p;
    printf("[Person] ID: %d, Name: %s, Age: %d\n", x->id, x->full_name ? x->full_name : "N/A", x->age);
}