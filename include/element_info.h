#ifndef ELEMENT_INFO_H
#define ELEMENT_INFO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>

typedef enum {
    TYPE_ANY = 0, TYPE_INT, TYPE_DOUBLE,
    TYPE_PERSON, TYPE_STUDENT, TYPE_TEACHER
} ElementType;

typedef struct ElementInfo {
    size_t element_size;
    void (*destroy)(void*);
    void* (*copy)(const void*);
    int (*compare)(const void*, const void*);
    void (*print)(const void*);
    ElementType type;
} ElementInfo;

ElementInfo* element_info_int(void);
ElementInfo* element_info_double(void);
ElementInfo* element_info_person(void);
ElementInfo* element_info_student(void);
ElementInfo* element_info_teacher(void);
bool element_info_is_compatible(const ElementInfo* a, const ElementInfo* b);

#ifdef __cplusplus
}
#endif

#endif 