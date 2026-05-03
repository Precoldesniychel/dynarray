#include <gtest/gtest.h>
#include "dynarray.h"
#include "element_info.h"
#include "polymorphic_record.h"


static PersonRecord* make_student(const char* name, int age, int id, double gpa) {
    return record_create_student(name, age, id, gpa);
}


static PersonRecord* make_teacher(const char* name, int age, int id, int exp, const char* subj) {
    return record_create_teacher(name, age, id, exp, subj);
}


TEST(PolymorphismTest, MixedArray) {
    DynArray* arr = array_create(element_info_person_record());
    PersonRecord* s = make_student("Stud", 20, 1, 4.0);
    PersonRecord* t = make_teacher("Teach", 50, 2, 5, "Bio");
    array_push(arr, s); array_push(arr, t);
    record_destroy(s); record_destroy(t);

    EXPECT_EQ(static_cast<PersonRecord*>(array_get(arr, 0))->type, RECORD_STUDENT);
    EXPECT_EQ(static_cast<PersonRecord*>(array_get(arr, 1))->type, RECORD_TEACHER);
    array_destroy(arr);
}

TEST(PolymorphismTest, TypeInfoSameSizeCompatible) {
    ElementInfo* info = element_info_person_record();
    EXPECT_EQ(info->element_size, sizeof(PersonRecord));
    int compatible = element_info_is_compatible(info, info);
    EXPECT_NE(compatible, 0);
}