#include <gtest/gtest.h>
#include "dynarray.h"
#include "student.h"
#include "teacher.h"

TEST(PolymorphismTest, MixedArrayOperations) {
    DynArray* people = array_create(element_info_person());
    
    Student* s = student_create("Ivanov", 20, 1, 4.8);
    Teacher* t = teacher_create("Petrov", 50, 2, 15, "Math");
    
    ASSERT_EQ(array_push(people, s), 0);
    ASSERT_EQ(array_push(people, t), 0);
    EXPECT_EQ(array_size(people), 2);
    
    // Проверяем полиморфный вывод (визуально в консоли)
    for(size_t i=0; i<array_size(people); ++i) {
        void* p = array_get(people, i);
        ASSERT_NE(p, nullptr);
    }
    
    array_destroy(people);
}

TEST(PolymorphismTest, TypeInfoCompatibility) {
    EXPECT_TRUE(element_info_is_compatible(element_info_person(), element_info_student()));
    EXPECT_TRUE(element_info_is_compatible(element_info_student(), element_info_person()));
    EXPECT_FALSE(element_info_is_compatible(element_info_int(), element_info_double()));
}