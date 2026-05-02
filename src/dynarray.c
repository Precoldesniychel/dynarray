#include "dynarray.h"
#include <stdlib.h>
#include <string.h>

// Вспомогательная: вычисляет смещение к элементу в байтах
static inline size_t _offset(const DynArray* arr, size_t index) {
    return index * arr->info->element_size;
}

// Вспомогательная: расширяет буфер
static int _grow(DynArray* arr) {
    size_t new_cap = (arr->capacity == 0) ? 4 : arr->capacity * 2;
    size_t new_bytes = new_cap * arr->info->element_size;
    void* new_data = realloc(arr->data, new_bytes);
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
    arr->data = NULL;
    arr->size = 0;
    arr->capacity = 0;
    return arr;
}

void array_destroy(DynArray* arr) {
    if (!arr) return;
    if (arr->info && arr->info->destroy && arr->data) {
        for (size_t i = 0; i < arr->size; ++i) {
            void* elem = (char*)arr->data + _offset(arr, i);
            arr->info->destroy(elem);
        }
    }
    free(arr->data);
    free(arr);
}

int array_push(DynArray* arr, const void* src) {
    if (!arr || !src || !arr->info || !arr->info->clone) return -1;
    if (arr->size >= arr->capacity && _grow(arr) != 0) return -1;
    
    // Клонируем объект во временную память
    void* clone = arr->info->clone(src);
    if (!clone) return -1;
    
    // Копируем байты клона в единый буфер массива
    void* dest = (char*)arr->data + _offset(arr, arr->size);
    memcpy(dest, clone, arr->info->element_size);
    
    // Освобождаем временный клон (теперь данные в буфере массива)
    free(clone);
    
    arr->size++;
    return 0;
}

void* array_get(const DynArray* arr, size_t index) {
    if (!arr || index >= arr->size) return NULL;
    return (char*)arr->data + _offset(arr, index);
}

size_t array_size(const DynArray* arr) { return arr ? arr->size : 0; }
size_t array_capacity(const DynArray* arr) { return arr ? arr->capacity : 0; }

int array_clear(DynArray* arr) {
    if (!arr || !arr->info || !arr->info->destroy || !arr->data) return -1;
    for (size_t i = 0; i < arr->size; ++i) {
        void* elem = (char*)arr->data + _offset(arr, i);
        arr->info->destroy(elem);
    }
    arr->size = 0;
    return 0;
}

int array_remove_at(DynArray* arr, size_t index) {
    if (!arr || index >= arr->size || !arr->data) return -1;
    
    // Уничтожаем удаляемый объект
    void* elem = (char*)arr->data + _offset(arr, index);
    if (arr->info->destroy) arr->info->destroy(elem);
    
    // Сдвигаем оставшиеся объекты
    size_t remaining = arr->size - index - 1;
    if (remaining > 0) {
        void* src = (char*)arr->data + _offset(arr, index + 1);
        void* dst = (char*)arr->data + _offset(arr, index);
        memmove(dst, src, remaining * arr->info->element_size);
    }
    arr->size--;
    return 0;
}

DynArray* array_map(const DynArray* arr, MapFunc func) {
    if (!arr || !func || !arr->info) return NULL;
    DynArray* res = array_create(arr->info);
    if (!res) return NULL;
    
    for (size_t i = 0; i < arr->size; ++i) {
        void* elem = array_get(arr, i);
        void* transformed = func(elem);
        if (!transformed || array_push(res, transformed) != 0) {
            if (transformed) arr->info->destroy(transformed);
            array_destroy(res);
            return NULL;
        }
        arr->info->destroy(transformed); // func вернула новый объект, push скопировал его в буфер
    }
    return res;
}

DynArray* array_where(const DynArray* arr, WhereFunc predicate) {
    if (!arr || !predicate || !arr->info) return NULL;
    DynArray* res = array_create(arr->info);
    if (!res) return NULL;
    
    for (size_t i = 0; i < arr->size; ++i) {
        void* elem = array_get(arr, i);
        if (predicate(elem)) {
            if (array_push(res, elem) != 0) {
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
        if (array_push(res, array_get(a, i)) != 0) { array_destroy(res); return NULL; }
    }
    for (size_t i = 0; i < b->size; ++i) {
        if (array_push(res, array_get(b, i)) != 0) { array_destroy(res); return NULL; }
    }
    return res;
}