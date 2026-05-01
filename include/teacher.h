#ifndef TEACHER_H
#define TEACHER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "person.h"

typedef struct Teacher { Person base; int experience_years; char* subject; } Teacher;

Teacher* teacher_create(const char* full_name, int age, int id, int exp, const char* subject);
void teacher_destroy(Teacher* t);
void* teacher_copy(const void* src);
int teacher_compare(const void* a, const void* b);
void teacher_print(const void* t);
int teacher_is_senior(const Teacher* t);

#ifdef __cplusplus
}
#endif

#endif // TEACHER_H