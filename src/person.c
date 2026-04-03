#include "../include/person.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Person* person_create(PersonType type,
                      PersonID id,
                      const char* firstName,
                      const char* middleName,
                      const char* lastName,
                      time_t birthDate) {
    Person* p = calloc(1, sizeof(Person));
    if (!p) return NULL;
    
    p->type = type;
    p->id = id;
    p->firstName = firstName ? strdup(firstName) : NULL;
    p->middleName = middleName ? strdup(middleName) : NULL;
    p->lastName = lastName ? strdup(lastName) : NULL;
    p->birthDate = birthDate;
    
    p->print = person_print;
    p->destroy = person_destroy;
    p->compare = person_compare;
    
    return p;
}

void person_destroy(Person* p) {
    if (!p) return;
    free(p->firstName);
    free(p->middleName);
    free(p->lastName);
    free(p);
}

void person_print(Person* p, FILE* out) {
    if (!p || !out) return;
    fprintf(out, "%s %s %s (ID: %d %d)", 
            p->lastName ? p->lastName : "", 
            p->firstName ? p->firstName : "", 
            p->middleName ? p->middleName : "",
            p->id.series, p->id.number);
}

int person_compare(Person* a, Person* b) {
    if (!a && !b) return 0;
    if (!a) return -1;
    if (!b) return 1;
    
    int cmp = strcmp(a->lastName ? a->lastName : "", b->lastName ? b->lastName : "");
    if (cmp) return cmp;
    cmp = strcmp(a->firstName ? a->firstName : "", b->firstName ? b->firstName : "");
    if (cmp) return cmp;
    return (a->id.series != b->id.series) ? 
           (a->id.series - b->id.series) : 
           (a->id.number - b->id.number);
}

const char* person_get_full_name(Person* p) {
    static char buffer[256];
    if (!p) return "";
    snprintf(buffer, sizeof(buffer), "%s %s %s", 
             p->lastName ? p->lastName : "", 
             p->firstName ? p->firstName : "", 
             p->middleName ? p->middleName : "");
    return buffer;
}

PersonType person_get_type(const Person* p) {
    return p ? p->type : PERSON_STUDENT;
}