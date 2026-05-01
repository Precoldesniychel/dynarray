#define _CRT_SECURE_NO_WARNINGS
#include "person.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Person* person_create(const char* name, int age, int id) {
    Person* p = (Person*)calloc(1, sizeof(Person));
    if (!p) return NULL;
    if (name) p->full_name = _strdup(name);
    p->age = age;
    p->id = id;
    return p;
}

void person_destroy(Person* p) {
    if (p) { free(p->full_name); free(p); }
}

void* person_copy(const void* src) {
    if (!src) return NULL;
    const Person* s = (const Person*)src;
    return person_create(s->full_name, s->age, s->id);
}

int person_compare(const void* a, const void* b) {
    if (!a || !b) return 0;
    const Person* pa = (const Person*)a;
    const Person* pb = (const Person*)b;
    const char* name_a = pa->full_name ? pa->full_name : "";
    const char* name_b = pb->full_name ? pb->full_name : "";
    int c = strcmp(name_a, name_b);
    return (c != 0) ? c : (pa->id - pb->id);
}

void person_print(const void* p) {
    if (!p) return;
    const Person* x = (const Person*)p;
    printf("[Person] ID: %d, Name: %s, Age: %d\n",
           x->id, x->full_name ? x->full_name : "N/A", x->age);
}