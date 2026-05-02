#include "student.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Student* student_create(const char* full_name, int age, int id, double gpa) {
    Student* s = malloc(sizeof(Student));
    if (!s) return NULL;
    if (full_name) {
        strncpy(s->base.full_name, full_name, sizeof(s->base.full_name) - 1);
        s->base.full_name[sizeof(s->base.full_name) - 1] = '\0';
    } else {
        s->base.full_name[0] = '\0';
    }
    s->base.age = age;
    s->base.id = id;
    s->gpa = gpa;
    return s;
}

void student_destroy(Student* s) {
    free(s);
}

void* student_clone(const void* src) {
    if (!src) return NULL;
    const Student* st = (const Student*)src;
    Student* s = malloc(sizeof(Student));
    if (!s) return NULL;
    memcpy(s, st, sizeof(Student));
    return s;
}

int student_compare(const void* a, const void* b) {
    if (!a || !b) return 0;
    const Student* sa = (const Student*)a;
    const Student* sb = (const Student*)b;
    if (sa->gpa < sb->gpa) return -1;
    if (sa->gpa > sb->gpa) return 1;
    return person_compare(&sa->base, &sb->base);
}

void student_print(const void* s) {
    if (!s) return;
    const Student* x = (const Student*)s;
    printf("[Student] ID: %d, Name: %s, Age: %d, GPA: %.2f\n",
           x->base.id, x->base.full_name, x->base.age, x->gpa);
}

int student_is_excellent(const Student* s) {
    return s && s->gpa >= 4.5;
}