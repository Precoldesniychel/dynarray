#ifndef DYNARRAY_H
#define DYNARRAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include "element_info.h"

typedef struct {
    void** data;
    size_t size;
    size_t capacity;
    ElementInfo* info;
} DynArray;

typedef void* (*MapFunc)(const void*);
typedef int   (*WhereFunc)(const void*);

DynArray* array_create(ElementInfo* info);
void array_destroy(DynArray* arr);

// Массив сам клонирует элемент и владеет им
int array_push(DynArray* arr, const void* src);
void* array_get(const DynArray* arr, size_t index);

size_t array_size(const DynArray* arr);
size_t array_capacity(const DynArray* arr);

int array_clear(DynArray* arr);
int array_remove_at(DynArray* arr, size_t index);

DynArray* array_map(const DynArray* arr, MapFunc func);
DynArray* array_where(const DynArray* arr, WhereFunc predicate);
DynArray* array_concat(const DynArray* a, const DynArray* b);

#ifdef __cplusplus
}
#endif

#endif // DYNARRAY_H