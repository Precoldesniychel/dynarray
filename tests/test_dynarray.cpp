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


TEST(DynArrayTest, CreateAndDestroy) {
    DynArray* arr = array_create(element_info_person_record());
    ASSERT_NE(arr, nullptr);
    EXPECT_EQ(array_size(arr), 0);
    array_destroy(arr);
}

TEST(DynArrayTest, PushAndGetStudent) {
    DynArray* arr = array_create(element_info_person_record());
    PersonRecord* s = make_student("Alice", 20, 1, 4.5);
    ASSERT_EQ(array_push(arr, s), 0);
    record_destroy(s);

    EXPECT_EQ(array_size(arr), 1);
    PersonRecord* retrieved = static_cast<PersonRecord*>(array_get(arr, 0));
    ASSERT_NE(retrieved, nullptr);
    EXPECT_EQ(retrieved->type, RECORD_STUDENT);
    EXPECT_STREQ(retrieved->full_name, "Alice");
    EXPECT_EQ(retrieved->age, 20);
    EXPECT_DOUBLE_EQ(retrieved->data.student.gpa, 4.5);

    array_destroy(arr);
}

TEST(DynArrayTest, PushAndGetTeacher) {
    DynArray* arr = array_create(element_info_person_record());
    PersonRecord* t = make_teacher("Bob", 50, 2, 20, "Math");
    ASSERT_EQ(array_push(arr, t), 0);
    record_destroy(t);

    PersonRecord* retrieved = static_cast<PersonRecord*>(array_get(arr, 0));
    EXPECT_EQ(retrieved->type, RECORD_TEACHER);
    EXPECT_STREQ(retrieved->full_name, "Bob");
    EXPECT_EQ(retrieved->data.teacher.experience_years, 20);
    EXPECT_STREQ(retrieved->data.teacher.subject, "Math");

    array_destroy(arr);
}

TEST(DynArrayTest, PushMultipleElements) {
    DynArray* arr = array_create(element_info_person_record());
    for (int i = 0; i < 100; ++i) {
        PersonRecord* s = make_student("Test", 20, i, 3.0);
        EXPECT_EQ(array_push(arr, s), 0);
        record_destroy(s);
    }
    EXPECT_EQ(array_size(arr), 100);
    PersonRecord* mid = static_cast<PersonRecord*>(array_get(arr, 50));
    EXPECT_EQ(mid->id, 50);
    array_destroy(arr);
}


TEST(DynArrayTest, CopyIndependence) {
    DynArray* arr = array_create(element_info_person_record());
    PersonRecord* s = make_student("Original", 18, 10, 3.0);
    array_push(arr, s);
    s->age = 99;
    strcpy(s->full_name, "Changed");
    record_destroy(s);

    PersonRecord* stored = static_cast<PersonRecord*>(array_get(arr, 0));
    EXPECT_STREQ(stored->full_name, "Original");
    EXPECT_EQ(stored->age, 18);
    array_destroy(arr);
}


TEST(DynArrayTest, RemoveAt) {
    DynArray* arr = array_create(element_info_person_record());
    for (int i = 1; i <= 5; ++i) {
        PersonRecord* s = make_student("S", 20, i, 3.0);
        array_push(arr, s);
        record_destroy(s);
    }

    EXPECT_EQ(array_remove_at(arr, 2), 0);
    EXPECT_EQ(array_size(arr), 4);
    EXPECT_EQ(static_cast<PersonRecord*>(array_get(arr, 0))->id, 1);
    EXPECT_EQ(static_cast<PersonRecord*>(array_get(arr, 1))->id, 2);
    EXPECT_EQ(static_cast<PersonRecord*>(array_get(arr, 2))->id, 4);
    EXPECT_EQ(static_cast<PersonRecord*>(array_get(arr, 3))->id, 5);

    EXPECT_EQ(array_remove_at(arr, 3), 0);
    EXPECT_EQ(array_size(arr), 3);
    EXPECT_EQ(array_remove_at(arr, 0), 0);
    EXPECT_EQ(static_cast<PersonRecord*>(array_get(arr, 0))->id, 2);

    array_destroy(arr);
}

TEST(DynArrayTest, Clear) {
    DynArray* arr = array_create(element_info_person_record());
    for (int i = 0; i < 5; ++i) {
        PersonRecord* s = make_student("X", 20, i, 3.0);
        array_push(arr, s);
        record_destroy(s);
    }
    EXPECT_EQ(array_clear(arr), 0);
    EXPECT_EQ(array_size(arr), 0);
    PersonRecord* s = make_student("New", 20, 99, 4.0);
    EXPECT_EQ(array_push(arr, s), 0);
    record_destroy(s);
    EXPECT_EQ(array_size(arr), 1);
    array_destroy(arr);
}