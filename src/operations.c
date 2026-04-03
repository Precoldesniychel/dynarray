#include "../include/dynarray.h"
#include <stdlib.h>
#include <string.h>

static inline size_t _offset(const DynArray* arr, size_t index) {
    return index * arr->info->element_size;
}

DynArray* array_map(const DynArray* src, MapFunc func, ElementInfo* result_info) {
    if (!src || !func || !result_info) {
        return NULL;
    }
    
    DynArray* result = array_create(src->size, result_info);
    if (!result) {
        return NULL;
    }
    
    for (size_t i = 0; i < src->size; i++) {
        const void* elem = (const char*)src->data + _offset(src, i);
        void* mapped = func(elem);
        
        if (mapped) {
            if (array_push(result, &mapped) < 0) {
                result_info->destroy(&mapped);
                array_destroy(result);
                return NULL;
            }
        }
    }
    
    return result;
}

DynArray* array_where(const DynArray* src, WhereFunc predicate) {
    if (!src || !predicate) {
        return NULL;
    }
    
    DynArray* result = array_create(src->size, src->info);
    if (!result) {
        return NULL;
    }
    
    for (size_t i = 0; i < src->size; i++) {
        const void* elem = (const char*)src->data + _offset(src, i);
        
        if (predicate(elem)) {
            if (array_push(result, elem) < 0) {
                array_destroy(result);
                return NULL;
            }
        }
    }
    
    return result;
}

DynArray* array_concat(const DynArray* a, const DynArray* b) {
    if (!a && !b) {
        return NULL;
    }
    if (!a) {
        return array_create(0, b->info);
    }
    if (!b) {
        return array_create(0, a->info);
    }
    
    if (!array_check_type_compatibility(a, b->info)) {
        return NULL;
    }
    
    size_t total_size = a->size + b->size;
    DynArray* result = array_create(total_size, a->info);
    if (!result) {
        return NULL;
    }
    
    for (size_t i = 0; i < a->size; i++) {
        const void* elem = (const char*)a->data + _offset(a, i);
        if (array_push(result, elem) < 0) {
            array_destroy(result);
            return NULL;
        }
    }
    
    for (size_t i = 0; i < b->size; i++) {
        const void* elem = (const char*)b->data + _offset(b, i);
        if (array_push(result, elem) < 0) {
            array_destroy(result);
            return NULL;
        }
    }
    
    return result;
}

void* array_reduce(const DynArray* src, ReduceFunc func, void* initial) {
    if (!src || !func) {
        return initial;
    }
    
    void* acc = initial;
    
    for (size_t i = 0; i < src->size; i++) {
        const void* elem = (const char*)src->data + _offset(src, i);
        acc = func(acc, elem);
    }
    
    return acc;
}