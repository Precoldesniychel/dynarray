#ifndef POLYMORPHIC_RECORD_H
#define POLYMORPHIC_RECORD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef enum {
    RECORD_STUDENT,
    RECORD_TEACHER
} RecordType;

typedef struct {
    char full_name[100];
    int age;
    int id;
    RecordType type;
    union {
        struct {
            double gpa;
        } student;
        struct {
            int experience_years;
            char subject[50];
        } teacher;
    } data;
} PersonRecord;


PersonRecord* record_create_student(const char* name, int age, int id, double gpa);
PersonRecord* record_create_teacher(const char* name, int age, int id, int exp, const char* subject);


void* record_clone(const void* src);
void  record_destroy(void* p);
int   record_compare(const void* a, const void* b);
char* record_to_string(const void* p);  

#ifdef __cplusplus
}
#endif

#endif