#include "../include/student.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Student* student_create(PersonID id,
                        const char* firstName,
                        const char* middleName,
                        const char* lastName,
                        time_t birthDate,
                        const char* group,
                        int course,
                        double average_grade) {
    Student* s = calloc(1, sizeof(Student));
    if (!s) return NULL;
    
    s->base = *person_create(PERSON_STUDENT, id, firstName, middleName, lastName, birthDate);
    s->base.print = (void(*)(Person*, FILE*))student_print;
    s->base.destroy = (void(*)(Person*))student_destroy;
    s->base.compare = (int(*)(Person*, Person*))student_compare;
    
    s->group = group ? strdup(group) : NULL;
    s->course = course;
    s->average_grade = average_grade;
    
    return s;
}

void student_destroy(Student* s) {
    if (!s) return;
    free(s->group);
    person_destroy(&s->base);
}

void student_print(Student* s, FILE* out) {
    if (!s || !out) return;
    person_print(&s->base, out);
    fprintf(out, " | Group: %s, Course: %d, GPA: %.2f", 
            s->group ? s->group : "N/A", s->course, s->average_grade);
}

int student_compare(Student* a, Student* b) {
    if (!a && !b) return 0;
    if (!a) return -1;
    if (!b) return 1;
    return person_compare(&a->base, &b->base);
}

int student_is_excellent(Student* s) {
    return s && s->average_grade >= 4.5;
}

void student_destroy_ptr(void* elem) {
    if (elem) {
        Student* s = *(Student**)elem;
        if (s) {
            student_destroy(s);
        }
    }
}

void student_print_ptr(const void* elem, FILE* out) {
    if (elem && out) {
        const Student* s = *(const Student* const*)elem;
        if (s) {
            student_print((Student*)s, out);
        } else {
            fprintf(out, "<NULL>");
        }
    }
}

int student_compare_ptr(const void* a, const void* b) {
    if (!a && !b) return 0;
    if (!a) return -1;
    if (!b) return 1;
    
    const Student* sa = *(const Student* const*)a;
    const Student* sb = *(const Student* const*)b;
    
    return student_compare((Student*)sa, (Student*)sb);
}