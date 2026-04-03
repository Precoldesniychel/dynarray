#ifndef PERSON_H
#define PERSON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "element_info.h"

typedef enum {
    PERSON_STUDENT = 0,
    PERSON_TEACHER = 1
} PersonType;

typedef struct {
    int series;
    int number;
} PersonID;

typedef struct Person {
    PersonType type;
    PersonID id;
    char* firstName;
    char* middleName;
    char* lastName;
    time_t birthDate;
    
    void (*print)(struct Person* self, FILE* out);
    void (*destroy)(struct Person* self);
    int (*compare)(struct Person* self, struct Person* other);
} Person;

Person* person_create(PersonType type,
                      PersonID id,
                      const char* firstName,
                      const char* middleName,
                      const char* lastName,
                      time_t birthDate);

void person_destroy(Person* p);

void person_print(Person* p, FILE* out);
int person_compare(Person* a, Person* b);
const char* person_get_full_name(Person* p);
PersonType person_get_type(const Person* p);

ElementInfo* get_person_ptr_info(void);

void person_destroy_ptr(void* elem);
void person_print_ptr(const void* elem, FILE* out);
int person_compare_ptr(const void* a, const void* b);

static inline PersonID person_get_id(const Person* p) {
    return p ? p->id : (PersonID){0, 0};
}

static inline const char* person_get_first_name(const Person* p) {
    return p && p->firstName ? p->firstName : "";
}

static inline const char* person_get_last_name(const Person* p) {
    return p && p->lastName ? p->lastName : "";
}

#endif