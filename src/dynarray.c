#include "dynarray.h"
#include <stdlib.h>   
#include <string.h>   

static int _grow(DynArray* arr) {
    if (!arr) return -1;
    size_t new_cap = (arr->capacity == 0) ? 4 : arr->capacity * 2;
    void** new_data = (void**)realloc(arr->data, new_cap * sizeof(void*));
    if (!new_data) return -1;
    arr->data = new_data;
    arr->capacity = new_cap;
    return 0;
}

DynArray* array_create(ElementInfo* info) {
    if (!info) return NULL;
    DynArray* arr = (DynArray*)calloc(1, sizeof(DynArray));
    if (!arr) return NULL;
    arr->element_info = info;
    return arr;
}

void array_destroy(DynArray* arr) {
    if (!arr) return;
    if (arr->element_info && arr->element_info->destroy) {
        for (size_t i = 0; i < arr->size; ++i) {
            if (arr->data[i]) arr->element_info->destroy(arr->data[i]);
        }
    }
    free(arr->data);
    free(arr);
}

int array_push(DynArray* arr, void* element) {
    if (!arr || !arr->element_info) return -1;
    if (arr->size >= arr->capacity && _grow(arr) != 0) return -1;
    arr->data[arr->size++] = element;
    return 0;
}

void* array_get(DynArray* arr, size_t index) {
    return (arr && index < arr->size) ? arr->data[index] : NULL;
}

const void* array_get_const(const DynArray* arr, size_t index) {
    return (arr && index < arr->size) ? arr->data[index] : NULL;
}

size_t array_size(const DynArray* arr) { return arr ? arr->size : 0; }
size_t array_capacity(const DynArray* arr) { return arr ? arr->capacity : 0; }

int array_clear(DynArray* arr) {
    if (!arr || !arr->element_info) return -1;
    if (arr->element_info->destroy) {
        for (size_t i = 0; i < arr->size; ++i) {
            if (arr->data[i]) arr->element_info->destroy(arr->data[i]);
        }
    }
    arr->size = 0;
    return 0;
}

int array_remove_at(DynArray* arr, size_t index) {
    if (!arr || index >= arr->size) return -1;
    if (arr->element_info && arr->element_info->destroy && arr->data[index]) {
        arr->element_info->destroy(arr->data[index]);
    }
    size_t remaining = (arr->size - index - 1) * sizeof(void*);
    if (remaining > 0) {
        memmove(&arr->data[index], &arr->data[index + 1], remaining);
    }
    arr->data[--arr->size] = NULL;
    return 0;
}

int array_set_null_at(DynArray* arr, size_t index) {
    if (!arr || index >= arr->size) return -1;
    if (arr->element_info && arr->element_info->destroy && arr->data[index]) {
        arr->element_info->destroy(arr->data[index]);
    }
    arr->data[index] = NULL;
    return 0;
}

bool array_is_null_at(const DynArray* arr, size_t index) {
    return (!arr || index >= arr->size) ? true : (arr->data[index] == NULL);
}
