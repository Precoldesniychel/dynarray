#include "element_info.h"
#include <stdlib.h>   
#include "person.h"   
#include "student.h"  
#include "teacher.h"  

static ElementInfo* _create_info(size_t sz, void (*d)(void*), void* (*c)(const void*),
                                 int (*cmp)(const void*, const void*), void (*p)(const void*), ElementType t) {
    ElementInfo* info = (ElementInfo*)calloc(1, sizeof(ElementInfo));
    if (!info) return NULL;
    info->element_size = sz; info->destroy = d; info->copy = c;
    info->compare = cmp; info->print = p; info->type = t;
    return info;
}

static ElementInfo* _int_info = NULL;
ElementInfo* element_info_int(void) {
    if (!_int_info) _int_info = _create_info(sizeof(int), NULL, NULL, NULL, NULL, TYPE_INT);
    return _int_info;
}

static ElementInfo* _double_info = NULL;
ElementInfo* element_info_double(void) {
    if (!_double_info) _double_info = _create_info(sizeof(double), NULL, NULL, NULL, NULL, TYPE_DOUBLE);
    return _double_info;
}

static ElementInfo* _person_info = NULL;
ElementInfo* element_info_person(void) {
    if (!_person_info) _person_info = _create_info(sizeof(Person), (void(*)(void*))person_destroy,
        (void*(*)(const void*))person_copy, (int(*)(const void*, const void*))person_compare,
        (void(*)(const void*))person_print, TYPE_PERSON);
    return _person_info;
}

static ElementInfo* _student_info = NULL;
ElementInfo* element_info_student(void) {
    if (!_student_info) _student_info = _create_info(sizeof(Student), (void(*)(void*))student_destroy,
        (void*(*)(const void*))student_copy, (int(*)(const void*, const void*))student_compare,
        (void(*)(const void*))student_print, TYPE_STUDENT);
    return _student_info;
}

static ElementInfo* _teacher_info = NULL;
ElementInfo* element_info_teacher(void) {
    if (!_teacher_info) _teacher_info = _create_info(sizeof(Teacher), (void(*)(void*))teacher_destroy,
        (void*(*)(const void*))teacher_copy, (int(*)(const void*, const void*))teacher_compare,
        (void(*)(const void*))teacher_print, TYPE_TEACHER);
    return _teacher_info;
}

bool element_info_is_compatible(const ElementInfo* a, const ElementInfo* b) {
    if (!a || !b) return false;
    if (a->type == b->type) return true;
    if ((a->type == TYPE_PERSON && (b->type == TYPE_STUDENT || b->type == TYPE_TEACHER)) ||
        (b->type == TYPE_PERSON && (a->type == TYPE_STUDENT || a->type == TYPE_TEACHER))) return true;
    return false;
}