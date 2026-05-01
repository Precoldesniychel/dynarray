#include "dynarray.h"
#include <stdlib.h>

DynArray* array_map(const DynArray* arr, MapFunc func) {
    if (!arr || !func || !arr->element_info) return NULL;
    DynArray* res = array_create(arr->element_info);
    if (!res) return NULL;
    for (size_t i = 0; i < arr->size; ++i) {
        void* new_elem = arr->data[i] ? func(arr->data[i]) : NULL;
        if (array_push(res, new_elem) != 0) {
            if (new_elem && arr->element_info->destroy) arr->element_info->destroy(new_elem);
            array_destroy(res);
            return NULL;
        }
    }
    return res;
}

DynArray* array_where(const DynArray* arr, WhereFunc predicate) {
    if (!arr || !predicate || !arr->element_info) return NULL;
    DynArray* res = array_create(arr->element_info);
    if (!res) return NULL;
    for (size_t i = 0; i < arr->size; ++i) {
        if (arr->data[i] && predicate(arr->data[i])) {
            if (array_push(res, arr->data[i]) != 0) {
                array_destroy(res);
                return NULL;
            }
        }
    }
    return res;
}

DynArray* array_concat(const DynArray* a, const DynArray* b) {
    if (!a || !b || !a->element_info || !b->element_info) return NULL;
    if (!element_info_is_compatible(a->element_info, b->element_info)) return NULL;
    
    DynArray* res = array_create(a->element_info);
    if (!res) return NULL;
    for (size_t i = 0; i < a->size; ++i) {
        if (array_push(res, a->data[i]) != 0) { array_destroy(res); return NULL; }
    }
    for (size_t i = 0; i < b->size; ++i) {
        if (array_push(res, b->data[i]) != 0) { array_destroy(res); return NULL; }
    }
    return res;
}

void* array_reduce(const DynArray* arr, ReduceFunc func, void* initial) {
    if (!arr || !func || !arr->element_info) return NULL;
    if (arr->size == 0) return initial;
    void* acc = initial;
    for (size_t i = 0; i < arr->size; ++i) {
        if (arr->data[i]) {
            acc = func(acc, arr->data[i]);
            if (!acc) return NULL;
        }
    }
    return acc;
}
