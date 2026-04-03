#ifndef STUDENT_H
#define STUDENT_H

#include "person.h"

typedef struct {
    Person base;
    char* group;
    int course;
    double average_grade;
} Student;

Student* student_create(PersonID id,
                        const char* firstName,
                        const char* middleName,
                        const char* lastName,
                        time_t birthDate,
                        const char* group,
                        int course,
                        double average_grade);

void student_destroy(Student* s);

void student_print(Student* s, FILE* out);
int student_compare(Student* a, Student* b);
int student_is_excellent(Student* s);

ElementInfo* get_student_ptr_info(void);

void student_destroy_ptr(void* elem);
void student_print_ptr(const void* elem, FILE* out);
int student_compare_ptr(const void* a, const void* b);

#endif