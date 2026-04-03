#ifndef DYNARRAY_H
#define DYNARRAY_H

#include <stddef.h>
#include <stdio.h>
#include "element_info.h"

typedef struct {
    void* data;
    size_t size;
    size_t capacity;
    ElementInfo* info;
} DynArray;

DynArray* array_create(size_t initial_capacity, ElementInfo* info);
void array_destroy(DynArray* arr);
int array_push(DynArray* arr, const void* element);
void* array_get(DynArray* arr, size_t index);
const void* array_get_const(const DynArray* arr, size_t index);
size_t array_size(const DynArray* arr);
size_t array_capacity(const DynArray* arr);
void array_print(const DynArray* arr, FILE* out);
void array_clear(DynArray* arr);
int array_remove_at(DynArray* arr, size_t index);
void array_set_null_at(DynArray* arr, size_t index);
int array_copy_element(DynArray* arr, size_t dest_index, const void* source);
int array_check_type_compatibility(const DynArray* arr, ElementInfo* expected_info);
int array_is_null_at(const DynArray* arr, size_t index);

typedef void* (*MapFunc)(const void* elem);
typedef int (*WhereFunc)(const void* elem);
typedef void* (*ReduceFunc)(void* acc, const void* elem);

DynArray* array_map(const DynArray* src, MapFunc func, ElementInfo* result_info);
DynArray* array_where(const DynArray* src, WhereFunc predicate);
DynArray* array_concat(const DynArray* a, const DynArray* b);
void* array_reduce(const DynArray* src, ReduceFunc func, void* initial);
void array_destroy_shallow(DynArray* arr);
#endif