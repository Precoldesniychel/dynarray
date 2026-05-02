#include "teacher.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Teacher* teacher_create(const char* full_name, int age, int id, int exp, const char* subject) {
    Teacher* t = malloc(sizeof(Teacher));
    if (!t) return NULL;
    if (full_name) {
        strncpy(t->base.full_name, full_name, sizeof(t->base.full_name) - 1);
        t->base.full_name[sizeof(t->base.full_name) - 1] = '\0';
    } else {
        t->base.full_name[0] = '\0';
    }
    t->base.age = age;
    t->base.id = id;
    t->experience_years = exp;
    if (subject) {
        strncpy(t->subject, subject, sizeof(t->subject) - 1);
        t->subject[sizeof(t->subject) - 1] = '\0';
    } else {
        t->subject[0] = '\0';
    }
    return t;
}

void teacher_destroy(Teacher* t) {
    free(t);
}

void* teacher_clone(const void* src) {
    if (!src) return NULL;
    const Teacher* tc = (const Teacher*)src;
    Teacher* t = malloc(sizeof(Teacher));
    if (!t) return NULL;
    memcpy(t, tc, sizeof(Teacher));
    return t;
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
           x->base.id, x->base.full_name, x->base.age,
           x->experience_years, x->subject);
}

int teacher_is_senior(const Teacher* t) {
    return t && t->experience_years >= 10;
}