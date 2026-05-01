#include "element_info.h"
#include <stdlib.h>
#include "person.h"
#include "student.h"
#include "teacher.h"

static ElementInfo* _create_info(size_t sz, void* (*cl)(const void*), void (*dst)(void*),
                                 int (*cmp)(const void*, const void*), void (*prt)(const void*), ElementType t) {
    ElementInfo* info = calloc(1, sizeof(ElementInfo));
    if (!info) return NULL;
    info->element_size = sz;
    info->clone = cl;
    info->destroy = dst;
    info->compare = cmp;
    info->print = prt;
    info->type = t;
    return info;
}


static ElementInfo* _person_info = NULL;
ElementInfo* element_info_person(void) {
    if (!_person_info) _person_info = _create_info(sizeof(Person), person_clone, (void(*)(void*))person_destroy, person_compare, person_print, TYPE_PERSON);
    return _person_info;
}

static ElementInfo* _student_info = NULL;
ElementInfo* element_info_student(void) {
    if (!_student_info) _student_info = _create_info(sizeof(Student), student_clone, (void(*)(void*))student_destroy, student_compare, student_print, TYPE_STUDENT);
    return _student_info;
}

static ElementInfo* _teacher_info = NULL;
ElementInfo* element_info_teacher(void) {
    if (!_teacher_info) _teacher_info = _create_info(sizeof(Teacher), teacher_clone, (void(*)(void*))teacher_destroy, teacher_compare, teacher_print, TYPE_TEACHER);
    return _teacher_info;
}

int element_info_is_compatible(const ElementInfo* a, const ElementInfo* b) {
    if (!a || !b) return 0;
    if (a == b) return 1;
    return (a->type == TYPE_PERSON && (b->type == TYPE_STUDENT || b->type == TYPE_TEACHER)) ||
           (b->type == TYPE_PERSON && (a->type == TYPE_STUDENT || a->type == TYPE_TEACHER));
}