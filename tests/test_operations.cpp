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

TEST(MapTest, IncrementGPAStudentsOnly) {
    DynArray* arr = array_create(element_info_person_record());
    PersonRecord* s1 = make_student("S1", 20, 1, 4.0);
    PersonRecord* t1 = make_teacher("T1", 40, 2, 10, "Phys");
    PersonRecord* s2 = make_student("S2", 21, 3, 4.9);
    array_push(arr, s1); array_push(arr, t1); array_push(arr, s2);
    record_destroy(s1); record_destroy(t1); record_destroy(s2);

    DynArray* mapped = array_map(arr, [](const void* elem) -> void* {
        const PersonRecord* rec = static_cast<const PersonRecord*>(elem);
        if (rec->type == RECORD_STUDENT) {
            PersonRecord* copy = static_cast<PersonRecord*>(record_clone(rec));
            copy->data.student.gpa += 0.1;
            if (copy->data.student.gpa > 5.0) copy->data.student.gpa = 5.0;
            return copy;
        }
        return record_clone(rec);
    });

    ASSERT_NE(mapped, nullptr);
    EXPECT_EQ(array_size(mapped), 3);
    auto* m1 = static_cast<PersonRecord*>(array_get(mapped, 0));
    EXPECT_EQ(m1->type, RECORD_STUDENT);
    EXPECT_DOUBLE_EQ(m1->data.student.gpa, 4.1);
    auto* m3 = static_cast<PersonRecord*>(array_get(mapped, 2));
    EXPECT_DOUBLE_EQ(m3->data.student.gpa, 5.0); 
    auto* m2 = static_cast<PersonRecord*>(array_get(mapped, 1));
    EXPECT_EQ(m2->type, RECORD_TEACHER);
    EXPECT_EQ(m2->data.teacher.experience_years, 10);

    array_destroy(mapped);
    array_destroy(arr);
}

TEST(MapTest, ChangeAgeAll) {
    DynArray* arr = array_create(element_info_person_record());
    PersonRecord* s = make_student("S", 20, 1, 3.0);
    PersonRecord* t = make_teacher("T", 50, 2, 5, "Eng");
    array_push(arr, s); array_push(arr, t);
    record_destroy(s); record_destroy(t);

    DynArray* mapped = array_map(arr, [](const void* elem) -> void* {
        const PersonRecord* rec = static_cast<const PersonRecord*>(elem);
        PersonRecord* copy = static_cast<PersonRecord*>(record_clone(rec));
        copy->age += 1;
        return copy;
    });
    ASSERT_NE(mapped, nullptr);
    EXPECT_EQ(static_cast<PersonRecord*>(array_get(mapped, 0))->age, 21);
    EXPECT_EQ(static_cast<PersonRecord*>(array_get(mapped, 1))->age, 51);

    array_destroy(mapped);
    array_destroy(arr);
}


TEST(WhereTest, ExcellentStudents) {
    DynArray* arr = array_create(element_info_person_record());
    PersonRecord* s1 = make_student("A", 20, 1, 4.5);
    PersonRecord* s2 = make_student("B", 21, 2, 4.4);
    PersonRecord* s3 = make_student("C", 22, 3, 4.6);
    PersonRecord* t1 = make_teacher("D", 40, 4, 15, "Math");
    array_push(arr, s1); array_push(arr, s2); array_push(arr, s3); array_push(arr, t1);
    record_destroy(s1); record_destroy(s2); record_destroy(s3); record_destroy(t1);

    DynArray* excellent = array_where(arr, [](const void* elem) -> int {
        const PersonRecord* rec = static_cast<const PersonRecord*>(elem);
        return rec->type == RECORD_STUDENT && rec->data.student.gpa >= 4.5;
    });
    ASSERT_NE(excellent, nullptr);
    EXPECT_EQ(array_size(excellent), 2);
    EXPECT_STREQ(static_cast<PersonRecord*>(array_get(excellent, 0))->full_name, "A");
    EXPECT_STREQ(static_cast<PersonRecord*>(array_get(excellent, 1))->full_name, "C");

    array_destroy(excellent);
    array_destroy(arr);
}

TEST(WhereTest, SeniorTeachers) {
    DynArray* arr = array_create(element_info_person_record());
    PersonRecord* t1 = make_teacher("Old", 60, 1, 30, "Hist");
    PersonRecord* t2 = make_teacher("Young", 35, 2, 2, "CS");
    PersonRecord* s = make_student("S", 20, 3, 4.0); 
    array_push(arr, t1); array_push(arr, t2); array_push(arr, s);
    record_destroy(t1); record_destroy(t2); record_destroy(s);

    DynArray* senior = array_where(arr, [](const void* elem) -> int {
        const PersonRecord* rec = static_cast<const PersonRecord*>(elem);
        return rec->type == RECORD_TEACHER && rec->data.teacher.experience_years >= 10;
    });
    ASSERT_NE(senior, nullptr);
    EXPECT_EQ(array_size(senior), 1);
    EXPECT_STREQ(static_cast<PersonRecord*>(array_get(senior, 0))->full_name, "Old");

    array_destroy(senior);
    array_destroy(arr);
}

TEST(WhereTest, EmptyResult) {
    DynArray* arr = array_create(element_info_person_record());
    PersonRecord* s = make_student("X", 20, 1, 3.0);
    array_push(arr, s);
    record_destroy(s);

    DynArray* res = array_where(arr, [](const void*) -> int { return 0; });
    ASSERT_NE(res, nullptr);
    EXPECT_EQ(array_size(res), 0);

    array_destroy(res);
    array_destroy(arr);
}



TEST(ConcatTest, MergeTwoArrays) {
    DynArray* a = array_create(element_info_person_record());
    DynArray* b = array_create(element_info_person_record());

    PersonRecord* s1 = make_student("S1", 20, 1, 4.0);
    PersonRecord* t1 = make_teacher("T1", 40, 2, 10, "Math");
    PersonRecord* s2 = make_student("S2", 22, 3, 4.5);
    array_push(a, s1); array_push(b, t1); array_push(b, s2);
    record_destroy(s1); record_destroy(t1); record_destroy(s2);

    DynArray* merged = array_concat(a, b);
    ASSERT_NE(merged, nullptr);
    EXPECT_EQ(array_size(merged), 3);
    EXPECT_EQ(static_cast<PersonRecord*>(array_get(merged, 0))->id, 1);
    EXPECT_EQ(static_cast<PersonRecord*>(array_get(merged, 1))->id, 2);
    EXPECT_EQ(static_cast<PersonRecord*>(array_get(merged, 2))->id, 3);

    array_destroy(merged);
    array_destroy(a);
    array_destroy(b);
}

TEST(ConcatTest, ConcatWithEmpty) {
    DynArray* a = array_create(element_info_person_record());
    DynArray* empty = array_create(element_info_person_record());
    PersonRecord* s = make_student("Only", 20, 1, 3.0);
    array_push(a, s);
    record_destroy(s);

    DynArray* res1 = array_concat(a, empty);
    EXPECT_EQ(array_size(res1), 1);
    array_destroy(res1);

    DynArray* res2 = array_concat(empty, a);
    EXPECT_EQ(array_size(res2), 1);
    array_destroy(res2);
    array_destroy(a);
    array_destroy(empty);
}


TEST(BoundaryTest, NullArguments) {
    DynArray* arr = array_create(element_info_person_record());
    EXPECT_EQ(array_push(nullptr, arr), -1);
    EXPECT_EQ(array_push(arr, nullptr), -1);
    EXPECT_EQ(array_get(nullptr, 0), nullptr);
    EXPECT_EQ(array_get(arr, 1000), nullptr);
    EXPECT_EQ(array_remove_at(nullptr, 0), -1);
    EXPECT_EQ(array_remove_at(arr, 0), -1); 
    EXPECT_EQ(array_map(nullptr, nullptr), nullptr);
    EXPECT_EQ(array_where(nullptr, nullptr), nullptr);
    EXPECT_EQ(array_concat(nullptr, arr), nullptr);
    array_destroy(arr);
}

TEST(BoundaryTest, EmptyArrayOperations) {
    DynArray* arr = array_create(element_info_person_record());
    EXPECT_EQ(array_clear(arr), 0);
    EXPECT_EQ(array_size(arr), 0);
    EXPECT_EQ(array_remove_at(arr, 0), -1);
    array_destroy(arr);
}


TEST(OperationsTest, ConcatMixedTypesAndMap) {
    DynArray* arr_teachers = array_create(element_info_person_record());
    DynArray* arr_students = array_create(element_info_person_record());


    PersonRecord* t = record_create_teacher("Petr", 50, 1, 15, "Math");
    PersonRecord* s1 = record_create_student("Base", 30, 2, 3.0);
    PersonRecord* s2 = record_create_student("Math", 32, 3, 4.5);


    ASSERT_EQ(array_push(arr_teachers, t), 0);
    ASSERT_EQ(array_push(arr_students, s1), 0);
    ASSERT_EQ(array_push(arr_students, s2), 0);
    record_destroy(t);
    record_destroy(s1);
    record_destroy(s2);


    DynArray* merged = array_concat(arr_teachers, arr_students);
    ASSERT_NE(merged, nullptr);
    EXPECT_EQ(array_size(merged), 3);


    DynArray* mapped = array_map(merged, [](const void* elem) -> void* {
        const PersonRecord* rec = static_cast<const PersonRecord*>(elem);
        PersonRecord* copy = static_cast<PersonRecord*>(record_clone(rec));
        copy->age += 1;
        return copy;
    });
    ASSERT_NE(mapped, nullptr);

    const PersonRecord* r0 = static_cast<const PersonRecord*>(array_get(mapped, 0));
    EXPECT_EQ(r0->type, RECORD_TEACHER);
    EXPECT_STREQ(r0->full_name, "Petr");
    EXPECT_EQ(r0->age, 51);
    EXPECT_EQ(r0->data.teacher.experience_years, 15);
    EXPECT_STREQ(r0->data.teacher.subject, "Math");

    const PersonRecord* r1 = static_cast<const PersonRecord*>(array_get(mapped, 1));
    EXPECT_EQ(r1->type, RECORD_STUDENT);
    EXPECT_STREQ(r1->full_name, "Base");
    EXPECT_EQ(r1->age, 31);
    EXPECT_DOUBLE_EQ(r1->data.student.gpa, 3.0);

    const PersonRecord* r2 = static_cast<const PersonRecord*>(array_get(mapped, 2));
    EXPECT_EQ(r2->type, RECORD_STUDENT);
    EXPECT_STREQ(r2->full_name, "Math");
    EXPECT_EQ(r2->age, 33);
    EXPECT_DOUBLE_EQ(r2->data.student.gpa, 4.5);

    for (size_t i = 0; i < array_size(mapped); ++i)
         mapped->info->print(array_get(mapped, i));

    array_destroy(mapped);
    array_destroy(merged);
    array_destroy(arr_teachers);
    array_destroy(arr_students);
}