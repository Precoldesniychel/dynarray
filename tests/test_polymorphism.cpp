#include <gtest/gtest.h>
#include "dynarray.h"
#include "student.h"
#include "teacher.h"

TEST(PolymorphismTest, MixedArray) {
    DynArray* people = array_create(element_info_person());
    
    Student s = {{"Ivan", 20, 1}, 4.8};
    Teacher t = {{"Petr", 50, 2}, 15, "Math"};
    
    EXPECT_EQ(array_push(people, &s), 0);
    EXPECT_EQ(array_push(people, &t), 0);
    EXPECT_EQ(array_size(people), 2);
    
    Person* p1 = static_cast<Person*>(array_get(people, 0));
    ASSERT_NE(p1, nullptr);
    EXPECT_STREQ(p1->full_name, "Ivan");
    
    Person* p2 = static_cast<Person*>(array_get(people, 1));
    ASSERT_NE(p2, nullptr);
    EXPECT_STREQ(p2->full_name, "Petr");
    
    array_destroy(people);
}

TEST(PolymorphismTest, TypeInfoCompatibility) {
    EXPECT_EQ(element_info_is_compatible(element_info_person(), element_info_student()), 1);
    EXPECT_EQ(element_info_is_compatible(element_info_student(), element_info_person()), 1);
    EXPECT_EQ(element_info_is_compatible(element_info_person(), element_info_teacher()), 1);
}