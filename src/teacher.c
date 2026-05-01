#include "teacher.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Teacher* teacher_create(const char* full_name, int age, int id, int exp, const char* subject) {
    Teacher* t = malloc(sizeof(Teacher));
    if (!t) return NULL;
#ifdef _MSC_VER
    t->base.full_name = full_name ? _strdup(full_name) : NULL;
    t->subject = subject ? _strdup(subject) : NULL;
#else
    t->base.full_name = full_name ? strdup(full_name) : NULL;
    t->subject = subject ? strdup(subject) : NULL;
#endif
    t->base.age = age;
    t->base.id = id;
    t->experience_years = exp;
    return t;
}

void teacher_destroy(Teacher* t) {
    if (!t) return;
    free(t->base.full_name);
    free(t->subject);
    free(t);
}

void* teacher_clone(const void* src) {
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
           x->base.id, x->base.full_name ? x->base.full_name : "N/A", x->base.age, x->experience_years, x->subject ? x->subject : "N/A");
}

int teacher_is_senior(const Teacher* t) {
    return t && t->experience_years >= 10;
}