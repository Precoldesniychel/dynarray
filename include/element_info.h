#ifndef ELEMENT_INFO_H
#define ELEMENT_INFO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef enum {
    TYPE_PERSON_RECORD   
} ElementType;

typedef struct ElementInfo {
    size_t element_size;
    void* (*clone)(const void*);
    void  (*destroy)(void*);
    int   (*compare)(const void*, const void*);
    void  (*print)(const void*);
    ElementType type;
} ElementInfo;


ElementInfo* element_info_person_record(void);
int element_info_is_compatible(const ElementInfo* a, const ElementInfo* b);

#ifdef __cplusplus
}
#endif

#endif