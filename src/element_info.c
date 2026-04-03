#include "../include/element_info.h"
#include "../include/person.h"
#include "../include/student.h"
#include "../include/teacher.h"
#include <stdlib.h>
#include <string.h>

static ElementInfo* g_person_ptr_info = NULL;
static ElementInfo* g_student_ptr_info = NULL;
static ElementInfo* g_teacher_ptr_info = NULL;

ElementInfo* element_info_create(size_t elem_size,
                                  void (*destroy)(void*),
                                  int (*compare)(const void*, const void*),
                                  void (*print)(const void*, FILE*),
                                  const char* type_name) {
    if (elem_size == 0) {
        return NULL;
    }
    
    ElementInfo* info = calloc(1, sizeof(ElementInfo));
    if (!info) {
        return NULL;
    }
    
    info->element_size = elem_size;
    info->destroy = destroy;
    info->compare = compare;
    info->print = print;
    
    if (type_name) {
        info->type_name = strdup(type_name);
        if (!info->type_name) {
            free(info);
            return NULL;
        }
    } else {
        info->type_name = NULL;
    }
    
    return info;
}

void element_info_destroy(ElementInfo* info) {
    if (!info) {
        return;
    }
    
    free((void*)info->type_name);
    free(info);
}

ElementInfo* get_person_ptr_info(void) {
    if (g_person_ptr_info == NULL) {
        g_person_ptr_info = element_info_create(
            sizeof(Person*),
            (void(*)(void*))person_destroy_ptr,
            (int(*)(const void*, const void*))person_compare_ptr,
            (void(*)(const void*, FILE*))person_print_ptr,
            "Person*"
        );
    }
    return g_person_ptr_info;
}

ElementInfo* get_student_ptr_info(void) {
    if (g_student_ptr_info == NULL) {
        g_student_ptr_info = element_info_create(
            sizeof(Student*),
            (void(*)(void*))student_destroy_ptr,
            (int(*)(const void*, const void*))student_compare_ptr,
            (void(*)(const void*, FILE*))student_print_ptr,
            "Student*"
        );
    }
    return g_student_ptr_info;
}

ElementInfo* get_teacher_ptr_info(void) {
    if (g_teacher_ptr_info == NULL) {
        g_teacher_ptr_info = element_info_create(
            sizeof(Teacher*),
            (void(*)(void*))teacher_destroy_ptr,
            (int(*)(const void*, const void*))teacher_compare_ptr,
            (void(*)(const void*, FILE*))teacher_print_ptr,
            "Teacher*"
        );
    }
    return g_teacher_ptr_info;
}

void person_destroy_ptr(void* elem) {
    if (elem) {
        Person* p = *(Person**)elem;
        if (p) {
            person_destroy(p);
        }
    }
}

void person_print_ptr(const void* elem, FILE* out) {
    if (elem && out) {
        const Person* p = *(const Person* const*)elem;
        if (p) {
            person_print((Person*)p, out);
        } else {
            fprintf(out, "<NULL>");
        }
    }
}

int person_compare_ptr(const void* a, const void* b) {
    if (!a && !b) return 0;
    if (!a) return -1;
    if (!b) return 1;
    
    const Person* pa = *(const Person* const*)a;
    const Person* pb = *(const Person* const*)b;
    
    return person_compare((Person*)pa, (Person*)pb);
}

int element_info_compare(const ElementInfo* a, const ElementInfo* b) {
    if (!a && !b) return 0;
    if (!a) return -1;
    if (!b) return 1;
    
    if (a->element_size != b->element_size) {
        return (a->element_size < b->element_size) ? -1 : 1;
    }
    
    if (a->destroy != b->destroy) return -1;
    if (a->compare != b->compare) return -1;
    if (a->print != b->print) return -1;
    
    return 0;
}

int element_info_is_compatible(const ElementInfo* a, const ElementInfo* b) {
    if (!a || !b) return 0;
    
    if (a == b) return 1;
    
    if (a == get_person_ptr_info()) {
        return (b == get_person_ptr_info() ||
                b == get_student_ptr_info() ||
                b == get_teacher_ptr_info());
    }
    
    return 0;
}