#include "element_info.h"
#include "polymorphic_record.h"
#include <stdlib.h>

static ElementInfo* _create_info(size_t sz, void* (*cl)(const void*), void (*dst)(void*),
                                 int (*cmp)(const void*, const void*), void (*prt)(const void*), ElementType t) {
    ElementInfo* info = (ElementInfo*)calloc(1, sizeof(ElementInfo));
    if (!info) return NULL;
    info->element_size = sz;
    info->clone = cl;
    info->destroy = dst;
    info->compare = cmp;
    info->print = prt;
    info->type = t;
    return info;
}

static ElementInfo* _record_info = NULL;
ElementInfo* element_info_person_record(void) {
    if (!_record_info) {
        _record_info = _create_info(sizeof(PersonRecord),
                                    record_clone,
                                    record_destroy,
                                    record_compare,
                                    record_print,
                                    TYPE_PERSON_RECORD);
    }
    return _record_info;
}

int element_info_is_compatible(const ElementInfo* a, const ElementInfo* b) {
    if (!a || !b) return 0;
    return (a->element_size == b->element_size);
}