#include "../include/dynarray.h"
#include <stdlib.h>
#include <string.h>

#define DEFAULT_CAPACITY 8

static int _grow(DynArray* arr) {
    if (!arr || !arr->info) {
        return -1;
    }
    
    size_t new_capacity = arr->capacity * 2;
    size_t new_bytes = new_capacity * arr->info->element_size;
    
    void* new_data = realloc(arr->data, new_bytes);
    if (!new_data) {
        return -1;
    }
    
    arr->data = new_data;
    arr->capacity = new_capacity;
    return 0;
}

static inline size_t _offset(const DynArray* arr, size_t index) {
    return index * arr->info->element_size;
}

DynArray* array_create(size_t initial_capacity, ElementInfo* info) {
    if (!info || info->element_size == 0) {
        return NULL;
    }
    
    DynArray* arr = calloc(1, sizeof(DynArray));
    if (!arr) {
        return NULL;
    }
    
    arr->capacity = (initial_capacity > 0) ? initial_capacity : DEFAULT_CAPACITY;
    arr->data = calloc(arr->capacity, info->element_size);
    if (!arr->data) {
        free(arr);
        return NULL;
    }
    
    arr->info = info;
    arr->size = 0;
    
    return arr;
}

void array_destroy(DynArray* arr) {
    if (!arr) {
        return;
    }
    
    if (arr->info && arr->info->destroy) {
        for (size_t i = 0; i < arr->size; i++) {
            void* elem = (char*)arr->data + _offset(arr, i);
            arr->info->destroy(elem);
        }
    }
    
    free(arr->data);
    free(arr);
}

int array_push(DynArray* arr, const void* element) {
    if (!arr || !element || !arr->info) {
        return -1;
    }
    
    if (arr->size >= arr->capacity && _grow(arr) < 0) {
        return -1;
    }
    
    void* dest = (char*)arr->data + _offset(arr, arr->size);
    memcpy(dest, element, arr->info->element_size);
    
    arr->size++;
    return 0;
}

void* array_get(DynArray* arr, size_t index) {
    if (!arr || index >= arr->size) {
        return NULL;
    }
    return (char*)arr->data + _offset(arr, index);
}

const void* array_get_const(const DynArray* arr, size_t index) {
    if (!arr || index >= arr->size) {
        return NULL;
    }
    return (const char*)arr->data + _offset(arr, index);
}

size_t array_size(const DynArray* arr) {
    return arr ? arr->size : 0;
}

size_t array_capacity(const DynArray* arr) {
    return arr ? arr->capacity : 0;
}

void array_print(const DynArray* arr, FILE* out) {
    if (!arr || !out) {
        return;
    }
    
    const char* type_name = (arr->info && arr->info->type_name) 
                            ? arr->info->type_name 
                            : "unknown";
    
    fprintf(out, "Array[%zu] of %s (capacity=%zu):\n", 
            arr->size, type_name, arr->capacity);
    
    if (arr->info && arr->info->print) {
        for (size_t i = 0; i < arr->size; i++) {
            const void* elem = (const char*)arr->data + _offset(arr, i);
            fprintf(out, "  [%zu] ", i);
            arr->info->print(elem, out);
            fprintf(out, "\n");
        }
    } else {
        for (size_t i = 0; i < arr->size; i++) {
            const void* elem = (const char*)arr->data + _offset(arr, i);
            fprintf(out, "  [%zu] %p\n", i, elem);
        }
    }
}

void array_clear(DynArray* arr) {
    if (!arr) {
        return;
    }
    
    if (arr->info && arr->info->destroy) {
        for (size_t i = 0; i < arr->size; i++) {
            void* elem = (char*)arr->data + _offset(arr, i);
            arr->info->destroy(elem);
        }
    }
    
    arr->size = 0;
}

int array_remove_at(DynArray* arr, size_t index) {
    if (!arr || !arr->info || index >= arr->size) {
        return -1;
    }
    
    if (arr->info->destroy) {
        void* elem = (char*)arr->data + _offset(arr, index);
        arr->info->destroy(elem);
    }
    
    size_t elements_to_move = arr->size - index - 1;
    if (elements_to_move > 0) {
        void* src = (char*)arr->data + _offset(arr, index + 1);
        void* dest = (char*)arr->data + _offset(arr, index);
        memmove(dest, src, elements_to_move * arr->info->element_size);
    }
    
    arr->size--;
    
    void* last = (char*)arr->data + _offset(arr, arr->size);
    memset(last, 0, arr->info->element_size);
    
    return 0;
}

void array_set_null_at(DynArray* arr, size_t index) {
    if (!arr || index >= arr->size) {
        return;
    }
    
    void* elem = (char*)arr->data + _offset(arr, index);
    memset(elem, 0, arr->info->element_size);
}

int array_copy_element(DynArray* arr, size_t dest_index, const void* source) {
    if (!arr || !source || dest_index >= arr->size || !arr->info) {
        return -1;
    }
    
    void* dest = (char*)arr->data + _offset(arr, dest_index);
    memcpy(dest, source, arr->info->element_size);
    return 0;
}

int array_check_type_compatibility(const DynArray* arr, ElementInfo* expected_info) {
    if (!arr || !arr->info || !expected_info) {
        return 0;
    }
    
    return element_info_is_compatible(arr->info, expected_info);
}

int array_is_null_at(const DynArray* arr, size_t index) {
    if (!arr || index >= arr->size) {
        return 1;
    }
    
    const void* elem = (const char*)arr->data + _offset(arr, index);
    
    if (arr->info->element_size == sizeof(void*)) {
        return (*(const void* const*)elem == NULL);
    }
    
    const unsigned char* bytes = (const unsigned char*)elem;
    for (size_t i = 0; i < arr->info->element_size; i++) {
        if (bytes[i] != 0) {
            return 0;
        }
    }
    return 1;
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

void array_destroy_shallow(DynArray* arr) {
    if (!arr) {
        return;
    }
    
    free(arr->data);
    free(arr);
}