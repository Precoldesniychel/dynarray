#ifndef TEACHER_H
#define TEACHER_H

#include "person.h"

typedef struct {
    Person base;
    char* department;
    char* position;
    int experience_years;
    double salary;
} Teacher;

Teacher* teacher_create(PersonID id,
                        const char* firstName,
                        const char* middleName,
                        const char* lastName,
                        time_t birthDate,
                        const char* department,
                        const char* position,
                        int experience_years,
                        double salary);

void teacher_destroy(Teacher* t);

void teacher_print(Teacher* t, FILE* out);
int teacher_compare(Teacher* a, Teacher* b);
int teacher_is_senior(Teacher* t);

ElementInfo* get_teacher_ptr_info(void);

void teacher_destroy_ptr(void* elem);
void teacher_print_ptr(const void* elem, FILE* out);
int teacher_compare_ptr(const void* a, const void* b);

#endif