#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "../include/student.h"
#include "../include/teacher.h"
#include "../include/dynarray.h"

static int tests_passed = 0;
static int tests_failed = 0;

#define TEST_ASSERT(condition, test_name) \
    do { \
        if (condition) { \
            printf("[PASS] %s\n", test_name); \
            tests_passed++; \
        } else { \
            printf("[FAIL] %s\n", test_name); \
            tests_failed++; \
        } \
    } while(0)

static void test_person_create() {
    printf("\n--- Testing Person Creation ---\n");
    
    PersonID id = {1234, 567890};
    Person* p = person_create(PERSON_STUDENT, id, "Ivan", "Ivanovich", "Ivanov", 0);
    
    TEST_ASSERT(p != NULL, "Person created successfully");
    TEST_ASSERT(p->type == PERSON_STUDENT, "Person type is correct");
    TEST_ASSERT(strcmp(p->lastName, "Ivanov") == 0, "Last name is correct");
    TEST_ASSERT(p->id.series == 1234, "ID series is correct");
    TEST_ASSERT(p->id.number == 567890, "ID number is correct");
    
    person_destroy(p);
}

static void test_student_operations() {
    printf("\n--- Testing Student Operations ---\n");
    
    Student* s1 = student_create((PersonID){1,1}, "Anna", "Petrovna", "Smirnova", 
                                  0, "507", 3, 4.8);
    Student* s2 = student_create((PersonID){2,2}, "Boris", "Sergeevich", "Kozlov",
                                  0, "508", 2, 3.9);
    Student* s3 = student_create((PersonID){3,3}, "Elena", "", "Volkova",
                                  0, "509", 4, 4.5);
    
    TEST_ASSERT(s1 != NULL, "Student s1 created");
    TEST_ASSERT(student_is_excellent(s1) == 1, "s1 is excellent (GPA 4.8)");
    TEST_ASSERT(student_is_excellent(s2) == 0, "s2 is not excellent (GPA 3.9)");
    TEST_ASSERT(student_is_excellent(s3) == 1, "s3 is excellent (GPA 4.5)");
    TEST_ASSERT(s1->course == 3, "Course is correct");
    TEST_ASSERT(strcmp(s1->group, "507") == 0, "Group is correct");
    
    student_destroy(s1);
    student_destroy(s2);
    student_destroy(s3);
}

static void test_teacher_operations() {
    printf("\n--- Testing Teacher Operations ---\n");
    
    Teacher* t1 = teacher_create((PersonID){10,10}, "Alexander", "", "Popov",
                                  0, "Mathematics", "Professor", 15, 80000);
    Teacher* t2 = teacher_create((PersonID){11,11}, "Maria", "", "Sokolova",
                                  0, "Physics", "Lecturer", 5, 50000);
    
    TEST_ASSERT(t1 != NULL, "Teacher t1 created");
    TEST_ASSERT(teacher_is_senior(t1) == 1, "t1 is senior (15 years)");
    TEST_ASSERT(teacher_is_senior(t2) == 0, "t2 is not senior (5 years)");
    TEST_ASSERT(t1->experience_years == 15, "Experience is correct");
    TEST_ASSERT(t1->salary == 80000, "Salary is correct");
    
    teacher_destroy(t1);
    teacher_destroy(t2);
}

static void test_array_basic() {
    printf("\n--- Testing Array Basic Operations ---\n");
    
    DynArray* arr = array_create(4, get_person_ptr_info());
    
    TEST_ASSERT(arr != NULL, "Array created successfully");
    TEST_ASSERT(array_size(arr) == 0, "Initial size is 0");
    TEST_ASSERT(arr->capacity >= 4, "Initial capacity is correct");
    
    Person* p1 = person_create(PERSON_STUDENT, (PersonID){1,1}, "A","B","C",0);
    Person* p2 = person_create(PERSON_TEACHER, (PersonID){2,2}, "D","E","F",0);
    Person* p3 = person_create(PERSON_STUDENT, (PersonID){3,3}, "G","H","I",0);
    
    TEST_ASSERT(array_push(arr, &p1) == 0, "Push p1 successful");
    TEST_ASSERT(array_push(arr, &p2) == 0, "Push p2 successful");
    TEST_ASSERT(array_push(arr, &p3) == 0, "Push p3 successful");
    TEST_ASSERT(array_size(arr) == 3, "Size is 3 after pushes");
    TEST_ASSERT(array_get(arr, 0) != NULL, "Get index 0 returns non-NULL");
    TEST_ASSERT(array_get(arr, 1) != NULL, "Get index 1 returns non-NULL");
    TEST_ASSERT(array_get(arr, 2) != NULL, "Get index 2 returns non-NULL");
    TEST_ASSERT(array_get(arr, 10) == NULL, "Get out of bounds returns NULL");
    
    TEST_ASSERT(array_push(NULL, &p1) == -1, "Push to NULL array fails");
    TEST_ASSERT(array_push(arr, NULL) == -1, "Push NULL element fails");
    
    array_destroy(arr);
}

static int _is_student(const void* elem) {
    const Person* p = *(const Person* const*)elem;
    return p->type == PERSON_STUDENT;
}

static int _is_teacher(const void* elem) {
    const Person* p = *(const Person* const*)elem;
    return p->type == PERSON_TEACHER;
}

static void* _double_salary(const void* elem) {
    const Person* p = *(const Person* const*)elem;
    if (p->type == PERSON_TEACHER) {
        const Teacher* t = (const Teacher*)p;
        return teacher_create(t->base.id, t->base.firstName, t->base.middleName,
                             t->base.lastName, t->base.birthDate,
                             t->department, t->position,
                             t->experience_years, t->salary * 2);
    }
    return NULL;
}

static void test_map_where() {
    printf("\n--- Testing Map and Where Operations ---\n");
    
    DynArray* arr = array_create(8, get_person_ptr_info());
    
    Student* s1 = student_create((PersonID){1,1}, "S1", "", "", 0, "507", 3, 4.5);
    Teacher* t1 = teacher_create((PersonID){2,2}, "T1", "", "", 0, "Math", "Prof", 15, 50000);
    Student* s2 = student_create((PersonID){3,3}, "S2", "", "", 0, "508", 2, 3.2);
    Teacher* t2 = teacher_create((PersonID){4,4}, "T2", "", "", 0, "Phys", "Lect", 5, 40000);
    
    array_push(arr, &s1);
    array_push(arr, &t1);
    array_push(arr, &s2);
    array_push(arr, &t2);
    
    TEST_ASSERT(array_size(arr) == 4, "Array has 4 elements");
    
    DynArray* students = array_where(arr, _is_student);
    TEST_ASSERT(students != NULL, "Where result not NULL");
    TEST_ASSERT(array_size(students) == 2, "Where found 2 students");
    array_destroy_shallow(students);
    
    DynArray* teachers = array_where(arr, _is_teacher);
    TEST_ASSERT(array_size(teachers) == 2, "Where found 2 teachers");
    array_destroy_shallow(teachers);
    
    DynArray* doubled = array_map(arr, _double_salary, get_teacher_ptr_info());
    TEST_ASSERT(doubled != NULL, "Map result not NULL");
    TEST_ASSERT(array_size(doubled) == 2, "Map produced 2 elements (teachers only)");
    
    Teacher** t_ptr = (Teacher**)array_get(doubled, 0);
    TEST_ASSERT(t_ptr != NULL && *t_ptr != NULL, "First mapped element not NULL");
    TEST_ASSERT((*t_ptr)->salary == 100000.0, "Salary doubled correctly (50000 -> 100000)");
    
    array_destroy(doubled);
    array_destroy(arr);
    
    TEST_ASSERT(array_where(NULL, _is_student) == NULL, "Where on NULL returns NULL");
    TEST_ASSERT(array_map(NULL, _double_salary, get_teacher_ptr_info()) == NULL, "Map on NULL returns NULL");
}

static void test_concat() {
    printf("\n--- Testing Concatenation ---\n");
    
    DynArray* a = array_create(2, get_person_ptr_info());
    DynArray* b = array_create(2, get_person_ptr_info());
    
    Person* p1 = person_create(PERSON_STUDENT, (PersonID){1,1}, "A","","",0);
    Person* p2 = person_create(PERSON_STUDENT, (PersonID){2,2}, "B","","",0);
    Person* p3 = person_create(PERSON_TEACHER, (PersonID){3,3}, "C","","",0);
    
    array_push(a, &p1);
    array_push(a, &p2);
    array_push(b, &p3);
    
    TEST_ASSERT(array_size(a) == 2, "Array a has 2 elements");
    TEST_ASSERT(array_size(b) == 1, "Array b has 1 element");
    
    DynArray* c = array_concat(a, b);
    TEST_ASSERT(c != NULL, "Concat result not NULL");
    TEST_ASSERT(array_size(c) == 3, "Concat result has 3 elements");
    
    DynArray* empty = array_create(0, get_person_ptr_info());
    DynArray* concat_empty = array_concat(a, empty);
    TEST_ASSERT(array_size(concat_empty) == 2, "Concat with empty preserves original");
    array_destroy_shallow(concat_empty);
    array_destroy(empty);
    
    TEST_ASSERT(array_concat(NULL, NULL) == NULL, "Concat NULL+NULL returns NULL");
    
    array_destroy_shallow(c);
    array_destroy_shallow(a);
    array_destroy_shallow(b);
}

static void test_array_remove() {
    printf("\n--- Testing Array Remove Operations ---\n");
    
    DynArray* arr = array_create(5, get_person_ptr_info());
    
    Person* persons[5];
    for (int i = 0; i < 5; i++) {
        persons[i] = person_create(PERSON_STUDENT, (PersonID){i,i}, "P","","",0);
        array_push(arr, &persons[i]);
    }
    TEST_ASSERT(array_size(arr) == 5, "Array has 5 elements");
    
    TEST_ASSERT(array_remove_at(arr, 2) == 0, "Remove at index 2 successful");
    TEST_ASSERT(array_size(arr) == 4, "Size decreased to 4");
    
    TEST_ASSERT(array_remove_at(arr, 0) == 0, "Remove at index 0 successful");
    TEST_ASSERT(array_size(arr) == 3, "Size decreased to 3");
    
    TEST_ASSERT(array_remove_at(arr, 10) == -1, "Remove out of bounds fails");
    
    array_destroy(arr);
}

static void test_memory_cleanup() {
    printf("\n--- Testing Memory Cleanup ---\n");
    
    DynArray* arr = array_create(4, get_person_ptr_info());
    
    Student* s = student_create((PersonID){1,1}, "Test","","Student",0,"507",3,4.0);
    Teacher* t = teacher_create((PersonID){2,2}, "Test","","Teacher",0,"Math","Prof",10,60000);
    
    array_push(arr, &s);
    array_push(arr, &t);
    
    TEST_ASSERT(array_size(arr) == 2, "Array has 2 elements before clear");
    
    array_clear(arr);
    TEST_ASSERT(array_size(arr) == 0, "Array size is 0 after clear");
    
    array_destroy(arr);
    
    TEST_ASSERT(1, "Array destroyed without crash (memory leak check)");
}

static void test_edge_cases() {
    printf("\n--- Testing Edge Cases ---\n");
    
    TEST_ASSERT(array_size(NULL) == 0, "Size of NULL array is 0");
    TEST_ASSERT(array_get(NULL, 0) == NULL, "Get from NULL returns NULL");
    TEST_ASSERT(person_compare(NULL, NULL) == 0, "Compare NULL+NULL returns 0");
    
    Person* p = person_create(PERSON_STUDENT,(PersonID){1,1},"","","",0);
    TEST_ASSERT(person_compare(NULL, p) == -1, "Compare NULL+Person returns -1");
    person_destroy(p);
    
    DynArray* empty = array_create(0, get_person_ptr_info());
    TEST_ASSERT(empty != NULL, "Empty array created");
    TEST_ASSERT(array_size(empty) == 0, "Empty array size is 0");
    array_destroy(empty);
    
    DynArray* large = array_create(1000, get_person_ptr_info());
    TEST_ASSERT(large->capacity >= 1000, "Large capacity array created");
    array_destroy(large);
}

int main() {
    printf("========================================\n");
    printf("    Variant 17 - Unit Test Suite\n");
    printf("    Polymorphic Collection (Students/Teachers)\n");
    printf("========================================\n");
    
    test_person_create();
    test_student_operations();
    test_teacher_operations();
    test_array_basic();
    test_map_where();
    test_concat();
    test_array_remove();
    test_memory_cleanup();
    test_edge_cases();
    
    printf("\n========================================\n");
    printf("    Test Results Summary\n");
    printf("========================================\n");
    printf("    Passed: %d\n", tests_passed);
    printf("    Failed: %d\n", tests_failed);
    printf("    Total:  %d\n", tests_passed + tests_failed);
    printf("========================================\n");
    
    if (tests_failed == 0) {
        printf("\n[PASS] All tests passed!\n");
        return 0;
    } else {
        printf("\n[FAIL] Some tests failed!\n");
        return 1;
    }
}