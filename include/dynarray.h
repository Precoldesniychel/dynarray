#ifndef DYNARRAY_H
#define DYNARRAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include "element_info.h"

typedef struct {
    void** data;
    size_t size;
    size_t capacity;
    ElementInfo* element_info;
} DynArray;

typedef void* (*MapFunc)(void*);
typedef bool (*WhereFunc)(const void*);
typedef void* (*ReduceFunc)(void*, void*);

DynArray* array_create(ElementInfo* info);
void array_destroy(DynArray* arr);

int array_push(DynArray* arr, void* element);
void* array_get(DynArray* arr, size_t index);
const void* array_get_const(const DynArray* arr, size_t index);

size_t array_size(const DynArray* arr);
size_t array_capacity(const DynArray* arr);

int array_clear(DynArray* arr);
int array_remove_at(DynArray* arr, size_t index);
int array_set_null_at(DynArray* arr, size_t index);
bool array_is_null_at(const DynArray* arr, size_t index);

DynArray* array_map(const DynArray* arr, MapFunc func);
DynArray* array_where(const DynArray* arr, WhereFunc predicate);
DynArray* array_concat(const DynArray* a, const DynArray* b);
void* array_reduce(const DynArray* arr, ReduceFunc func, void* initial);

#ifdef __cplusplus
}
#endif

#endif 