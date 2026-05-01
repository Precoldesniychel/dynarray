#include "dynarray.h"
#include <stdlib.h>
#include <string.h>

static int _grow(DynArray* arr) {
    size_t new_cap = (arr->capacity == 0) ? 4 : arr->capacity * 2;
    void** new_data = realloc(arr->data, new_cap * sizeof(void*));
    if (!new_data) return -1;
    arr->data = new_data;
    arr->capacity = new_cap;
    return 0;
}

DynArray* array_create(ElementInfo* info) {
    if (!info) return NULL;
    DynArray* arr = calloc(1, sizeof(DynArray));
    if (!arr) return NULL;
    arr->info = info;
    return arr;
}

void array_destroy(DynArray* arr) {
    if (!arr) return;
    if (arr->info && arr->info->destroy) {
        for (size_t i = 0; i < arr->size; ++i) {
            if (arr->data[i]) arr->info->destroy(arr->data[i]);
        }
    }
    free(arr->data);
    free(arr);
}

int array_push(DynArray* arr, const void* src) {
    if (!arr || !src || !arr->info || !arr->info->clone) return -1;
    if (arr->size >= arr->capacity && _grow(arr) != 0) return -1;
    
    void* clone = arr->info->clone(src);
    if (!clone) return -1;
    arr->data[arr->size++] = clone;
    return 0;
}

void* array_get(const DynArray* arr, size_t index) {
    if (!arr || index >= arr->size) return NULL;
    return arr->data[index];
}

size_t array_size(const DynArray* arr) { return arr ? arr->size : 0; }
size_t array_capacity(const DynArray* arr) { return arr ? arr->capacity : 0; }

int array_clear(DynArray* arr) {
    if (!arr || !arr->info || !arr->info->destroy) return -1;
    for (size_t i = 0; i < arr->size; ++i) {
        if (arr->data[i]) arr->info->destroy(arr->data[i]);
    }
    arr->size = 0;
    return 0;
}

int array_remove_at(DynArray* arr, size_t index) {
    if (!arr || index >= arr->size) return -1;
    if (arr->info->destroy && arr->data[index]) arr->info->destroy(arr->data[index]);
    
    size_t remaining = (arr->size - index - 1) * sizeof(void*);
    if (remaining > 0) memmove(&arr->data[index], &arr->data[index + 1], remaining);
    arr->data[--arr->size] = NULL;
    return 0;
}

DynArray* array_map(const DynArray* arr, MapFunc func) {
    if (!arr || !func || !arr->info) return NULL;
    DynArray* res = array_create(arr->info);
    if (!res) return NULL;
    
    for (size_t i = 0; i < arr->size; ++i) {
        void* new_elem = func(arr->data[i]);
        if (!new_elem || array_push(res, new_elem) != 0) {
            if (new_elem) arr->info->destroy(new_elem);
            array_destroy(res);
            return NULL;
        }
        arr->info->destroy(new_elem); 
    }
    return res;
}

DynArray* array_where(const DynArray* arr, WhereFunc predicate) {
    if (!arr || !predicate || !arr->info) return NULL;
    DynArray* res = array_create(arr->info);
    if (!res) return NULL;
    
    for (size_t i = 0; i < arr->size; ++i) {
        if (predicate(arr->data[i])) {
            if (array_push(res, arr->data[i]) != 0) {
                array_destroy(res);
                return NULL;
            }
        }
    }
    return res;
}

DynArray* array_concat(const DynArray* a, const DynArray* b) {
    if (!a || !b || !element_info_is_compatible(a->info, b->info)) return NULL;
    DynArray* res = array_create(a->info);
    if (!res) return NULL;
    
    for (size_t i = 0; i < a->size; ++i) {
        if (array_push(res, a->data[i]) != 0) { array_destroy(res); return NULL; }
    }
    for (size_t i = 0; i < b->size; ++i) {
        if (array_push(res, b->data[i]) != 0) { array_destroy(res); return NULL; }
    }
    return res;
}