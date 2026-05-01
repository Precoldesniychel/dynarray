#include <gtest/gtest.h>
#include "dynarray.h"
#include "student.h"
#include "teacher.h"

static int is_excellent(const void* p) {
    return static_cast<const Student*>(p)->gpa >= 4.5;
}

static int is_senior(const void* p) {
    return static_cast<const Teacher*>(p)->experience_years >= 10;
}

TEST(OperationsTest, WhereFilterStudents) {
    DynArray* arr = array_create(element_info_student());
    Student s1 = {{"A", 20, 1}, 3.0};
    Student s2 = {{"B", 20, 2}, 5.0};
    Student s3 = {{"C", 20, 3}, 4.0};
    
    array_push(arr, &s1);
    array_push(arr, &s2);
    array_push(arr, &s3);
    
    DynArray* filtered = array_where(arr, is_excellent);
    ASSERT_NE(filtered, nullptr);
    EXPECT_EQ(array_size(filtered), 1);
    
    Student* res = static_cast<Student*>(array_get(filtered, 0));
    EXPECT_STREQ(res->base.full_name, "B");
    
    array_destroy(filtered);
    array_destroy(arr);
}

TEST(OperationsTest, WhereFilterTeachers) {
    DynArray* arr = array_create(element_info_teacher());
    Teacher t1 = {{"X", 30, 1}, 5, "Chem"};
    Teacher t2 = {{"Y", 40, 2}, 12, "Math"};
    
    array_push(arr, &t1);
    array_push(arr, &t2);
    
    DynArray* filtered = array_where(arr, is_senior);
    ASSERT_NE(filtered, nullptr);
    EXPECT_EQ(array_size(filtered), 1);
    
    Teacher* res = static_cast<Teacher*>(array_get(filtered, 0));
    EXPECT_STREQ(res->base.full_name, "Y");
    
    array_destroy(filtered);
    array_destroy(arr);
}

TEST(OperationsTest, Concat) {
    DynArray* a = array_create(element_info_person());
    DynArray* b = array_create(element_info_person());
    
    Student s = {{"S", 20, 1}, 4.0};
    Teacher t = {{"T", 40, 2}, 10, "Math"};
    
    array_push(a, &s);
    array_push(b, &t);
    
    DynArray* c = array_concat(a, b);
    ASSERT_NE(c, nullptr);
    EXPECT_EQ(array_size(c), 2);
    EXPECT_EQ(static_cast<Person*>(array_get(c, 0))->age, 20);
    EXPECT_EQ(static_cast<Person*>(array_get(c, 1))->age, 40);
    
    array_destroy(c);
    array_destroy(a);
    array_destroy(b);
}