#include <gtest/gtest.h>
#include "dynarray.h"
#include "element_info.h"
#include "student.h" // Добавлено для типа Student

TEST(DynArrayTest, CreateAndDestroy) {
    DynArray* arr = array_create(element_info_student());
    ASSERT_NE(arr, nullptr);
    EXPECT_EQ(array_size(arr), 0);
    EXPECT_EQ(array_capacity(arr), 0);
    array_destroy(arr);
}

TEST(DynArrayTest, PushGetAndOwnership) {
    DynArray* arr = array_create(element_info_student());
    Student s = {{"Test Student", 20, 1}, 4.5};
    EXPECT_EQ(array_push(arr, &s), 0);
    
    EXPECT_EQ(array_size(arr), 1);
    EXPECT_GT(array_capacity(arr), 0);
    
    Student* retrieved = static_cast<Student*>(array_get(arr, 0));
    ASSERT_NE(retrieved, nullptr);
    EXPECT_STREQ(retrieved->base.full_name, "Test Student");
    EXPECT_DOUBLE_EQ(retrieved->gpa, 4.5); // Исправлено на EXPECT_DOUBLE_EQ
    EXPECT_NE(retrieved, &s); // Проверка владения копией
    
    array_destroy(arr);
}

TEST(DynArrayTest, BoundaryChecks) {
    DynArray* arr = array_create(element_info_person());
    EXPECT_EQ(array_get(arr, 0), nullptr);
    EXPECT_EQ(array_remove_at(arr, 5), -1);
    EXPECT_EQ(array_push(arr, NULL), -1);
    array_destroy(arr);
}