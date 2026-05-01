#define _CRT_SECURE_NO_WARNINGS
#include "student.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Student* student_create(const char* name, int age, int id, double gpa) {
    Student* s = (Student*)calloc(1, sizeof(Student));
    if (!s) return NULL;
    if (name) s->base.full_name = _strdup(name);
    s->base.age = age;
    s->base.id = id;
    s->gpa = gpa;
    return s;
}

void student_destroy(Student* s) {
    if (s) { free(s->base.full_name); free(s); }
}

void* student_copy(const void* src) {
    if (!src) return NULL;
    const Student* st = (const Student*)src;
    return student_create(st->base.full_name, st->base.age, st->base.id, st->gpa);
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
           x->base.id, x->base.full_name ? x->base.full_name : "N/A", x->base.age, x->gpa);
}

int student_is_excellent(const Student* s) {
    return s && s->gpa >= 4.5;
}