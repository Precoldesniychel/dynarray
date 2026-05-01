#define _CRT_SECURE_NO_WARNINGS
#include "teacher.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Teacher* teacher_create(const char* name, int age, int id, int exp, const char* subj) {
    Teacher* t = (Teacher*)calloc(1, sizeof(Teacher));
    if (!t) return NULL;
    if (name) t->base.full_name = _strdup(name);
    t->base.age = age;
    t->base.id = id;
    t->experience_years = exp;
    if (subj) t->subject = _strdup(subj);
    return t;
}

void teacher_destroy(Teacher* t) {
    if (t) { free(t->base.full_name); free(t->subject); free(t); }
}

void* teacher_copy(const void* src) {
    if (!src) return NULL;
    const Teacher* tc = (const Teacher*)src;
    return teacher_create(tc->base.full_name, tc->base.age, tc->base.id, tc->experience_years, tc->subject);
}

int teacher_compare(const void* a, const void* b) {
    if (!a || !b) return 0;
    const Teacher* ta = (const Teacher*)a;
    const Teacher* tb = (const Teacher*)b;
    if (ta->experience_years < tb->experience_years) return -1;
    if (ta->experience_years > tb->experience_years) return 1;
    return person_compare(&ta->base, &tb->base);
}

void teacher_print(const void* t) {
    if (!t) return;
    const Teacher* x = (const Teacher*)t;
    printf("[Teacher] ID: %d, Name: %s, Age: %d, Exp: %d, Subj: %s\n",
           x->base.id, x->base.full_name ? x->base.full_name : "N/A", x->base.age,
           x->experience_years, x->subject ? x->subject : "N/A");
}

int teacher_is_senior(const Teacher* t) {
    return t && t->experience_years >= 10;
}
