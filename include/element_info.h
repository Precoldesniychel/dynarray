#ifndef ELEMENT_INFO_H
#define ELEMENT_INFO_H

#include <stddef.h>
#include <stdio.h>

typedef struct {
    size_t element_size;
    void (*destroy)(void* elem);
    int (*compare)(const void* a, const void* b);
    void (*print)(const void* elem, FILE* out);
    const char* type_name;
} ElementInfo;

ElementInfo* element_info_create(size_t elem_size,
                                  void (*destroy)(void*),
                                  int (*compare)(const void*, const void*),
                                  void (*print)(const void*, FILE*),
                                  const char* type_name);

void element_info_destroy(ElementInfo* info);

ElementInfo* get_person_ptr_info(void);
ElementInfo* get_student_ptr_info(void);
ElementInfo* get_teacher_ptr_info(void);

int element_info_compare(const ElementInfo* a, const ElementInfo* b);
int element_info_is_compatible(const ElementInfo* a, const ElementInfo* b);

#endif