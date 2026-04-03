#include "../include/teacher.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Teacher* teacher_create(PersonID id,
                        const char* firstName,
                        const char* middleName,
                        const char* lastName,
                        time_t birthDate,
                        const char* department,
                        const char* position,
                        int experience_years,
                        double salary) {
    Teacher* t = calloc(1, sizeof(Teacher));
    if (!t) return NULL;
    
    t->base = *person_create(PERSON_TEACHER, id, firstName, middleName, lastName, birthDate);
    t->base.print = (void(*)(Person*, FILE*))teacher_print;
    t->base.destroy = (void(*)(Person*))teacher_destroy;
    t->base.compare = (int(*)(Person*, Person*))teacher_compare;
    
    t->department = department ? strdup(department) : NULL;
    t->position = position ? strdup(position) : NULL;
    t->experience_years = experience_years;
    t->salary = salary;
    
    return t;
}

void teacher_destroy(Teacher* t) {
    if (!t) return;
    free(t->department);
    free(t->position);
    person_destroy(&t->base);
}

void teacher_print(Teacher* t, FILE* out) {
    if (!t || !out) return;
    person_print(&t->base, out);
    fprintf(out, " | Department: %s, Position: %s, Experience: %d years, Salary: %.2f",
            t->department ? t->department : "N/A", 
            t->position ? t->position : "N/A",
            t->experience_years, t->salary);
}

int teacher_compare(Teacher* a, Teacher* b) {
    if (!a && !b) return 0;
    if (!a) return -1;
    if (!b) return 1;
    return person_compare(&a->base, &b->base);
}

int teacher_is_senior(Teacher* t) {
    return t && t->experience_years >= 10;
}

void teacher_destroy_ptr(void* elem) {
    if (elem) {
        Teacher* t = *(Teacher**)elem;
        if (t) {
            teacher_destroy(t);
        }
    }
}

void teacher_print_ptr(const void* elem, FILE* out) {
    if (elem && out) {
        const Teacher* t = *(const Teacher* const*)elem;
        if (t) {
            teacher_print((Teacher*)t, out);
        } else {
            fprintf(out, "<NULL>");
        }
    }
}

int teacher_compare_ptr(const void* a, const void* b) {
    if (!a && !b) return 0;
    if (!a) return -1;
    if (!b) return 1;
    
    const Teacher* ta = *(const Teacher* const*)a;
    const Teacher* tb = *(const Teacher* const*)b;
    
    return teacher_compare((Teacher*)ta, (Teacher*)tb);
}