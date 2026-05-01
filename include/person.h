#ifndef PERSON_H
#define PERSON_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Person {
    char* full_name;
    int age;
    int id;
} Person;

Person* person_create(const char* full_name, int age, int id);
void person_destroy(Person* p);
void* person_clone(const void* src);
int person_compare(const void* a, const void* b);
void person_print(const void* p);

#ifdef __cplusplus
}
#endif

#endif 