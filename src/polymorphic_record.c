#include "polymorphic_record.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void init_base(PersonRecord* rec, const char* name, int age, int id, RecordType type) {
    if (name) {
        strncpy(rec->full_name, name, sizeof(rec->full_name) - 1);
        rec->full_name[sizeof(rec->full_name) - 1] = '\0';
    } else {
        rec->full_name[0] = '\0';
    }
    rec->age = age;
    rec->id = id;
    rec->type = type;
}

PersonRecord* record_create_student(const char* name, int age, int id, double gpa) {
    PersonRecord* rec = (PersonRecord*)malloc(sizeof(PersonRecord));
    if (!rec) return NULL;
    init_base(rec, name, age, id, RECORD_STUDENT);
    rec->data.student.gpa = gpa;
    return rec;
}

PersonRecord* record_create_teacher(const char* name, int age, int id, int exp, const char* subject) {
    PersonRecord* rec = (PersonRecord*)malloc(sizeof(PersonRecord));
    if (!rec) return NULL;
    init_base(rec, name, age, id, RECORD_TEACHER);
    rec->data.teacher.experience_years = exp;
    if (subject) {
        strncpy(rec->data.teacher.subject, subject, sizeof(rec->data.teacher.subject) - 1);
        rec->data.teacher.subject[sizeof(rec->data.teacher.subject) - 1] = '\0';
    } else {
        rec->data.teacher.subject[0] = '\0';
    }
    return rec;
}

void* record_clone(const void* src) {
    if (!src) return NULL;
    PersonRecord* new_rec = (PersonRecord*)malloc(sizeof(PersonRecord));
    if (!new_rec) return NULL;
    memcpy(new_rec, src, sizeof(PersonRecord));
    return new_rec;
}

void record_destroy(void* p) {
    free(p);
}

int record_compare(const void* a, const void* b) {
    const PersonRecord* ra = (const PersonRecord*)a;
    const PersonRecord* rb = (const PersonRecord*)b;
    return ra->id - rb->id;
}

char* record_to_string(const void* p) {
    const PersonRecord* rec = (const PersonRecord*)p;
    if (!rec) return NULL;
    char buffer[512];
    if (rec->type == RECORD_STUDENT) {
        snprintf(buffer, sizeof(buffer), "[Student] ID: %d, Name: %s, Age: %d, GPA: %.2f",
                 rec->id, rec->full_name, rec->age, rec->data.student.gpa);
    } else {
        snprintf(buffer, sizeof(buffer), "[Teacher] ID: %d, Name: %s, Age: %d, Exp: %d, Subj: %s",
                 rec->id, rec->full_name, rec->age,
                 rec->data.teacher.experience_years, rec->data.teacher.subject);
    }
    char* result = (char*)malloc(strlen(buffer) + 1);
    if (result) strcpy(result, buffer);
    return result;
}