#include <gtest/gtest.h>
#include "dynarray.h"
#include "student.h"
#include "teacher.h"
#include "person.h"

TEST(OperationsTest, ConcatThreeTypesThenMap) {
    DynArray* arr_p = array_create(element_info_person());
    DynArray* arr_s = array_create(element_info_student());
    DynArray* arr_t = array_create(element_info_teacher());

    Person p  = {"Base", 30, 1};
    Student s = {{"Ivan", 20, 2}, 4.8};
    Teacher t = {{"Petr", 50, 3}, 15, "Math"};

    ASSERT_EQ(array_push(arr_p, &p), 0);
    ASSERT_EQ(array_push(arr_s, &s), 0);
    ASSERT_EQ(array_push(arr_t, &t), 0);

    DynArray* step1 = array_concat(arr_p, arr_s);
    DynArray* step2 = array_concat(step1, arr_t);
    
    DynArray* mapped = array_map(step2, [](const void* elem) -> void* {
        Person* copy = person_clone(elem);
        if (copy) copy->age += 1;
        return copy;
    });

    ASSERT_NE(mapped, nullptr);
    EXPECT_EQ(array_size(mapped), 3);

    // C-STYLE ПРИВЕДЕНИЕ: MSVC не ругается, работает идентично
    const Person* p1 = (const Person*)array_get(mapped, 0);
    const Student* p2 = (const Student*)array_get(mapped, 1);
    const Teacher* p3 = (const Teacher*)array_get(mapped, 2);

    EXPECT_EQ(p1->age, 31);
    EXPECT_EQ(p2->base.age, 21);
    EXPECT_EQ(p3->base.age, 51);

    array_destroy(mapped);
    array_destroy(step2);
    array_destroy(step1);
    array_destroy(arr_p);
    array_destroy(arr_s);
    array_destroy(arr_t);
}