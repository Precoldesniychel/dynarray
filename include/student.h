#ifndef STUDENT_H
#define STUDENT_H

#include "person.h"

typedef struct {
    Person base;
    double gpa;
} Student;

#ifdef __cplusplus
extern "C" {
#endif

Student* student_create(const char* full_name, int age, int id, double gpa);
void student_destroy(Student* s);
void* student_clone(const void* src);
int student_compare(const void* a, const void* b);
void student_print(const void* s);
int student_is_excellent(const Student* s);

#ifdef __cplusplus
}
#endif

#endif