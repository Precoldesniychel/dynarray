#include <gtest/gtest.h>
#include <stdlib.h>
#include "dynarray.h"
#include "student.h"
#include "person.h"


static bool is_excellent(const void* p) {
    return ((const Student*)p)->gpa >= 4.5;
}

TEST(OperationsTest, WhereFilter) {
    DynArray* arr = array_create(element_info_student());

    array_push(arr, student_create("A", 20, 1, 3.5));
    array_push(arr, student_create("B", 20, 2, 5.0));
    array_push(arr, student_create("C", 20, 3, 4.7));

    DynArray* filtered = array_where(arr, is_excellent);
    ASSERT_NE(filtered, nullptr);
    EXPECT_EQ(array_size(filtered), 2); 
    

    free(filtered->data);
    free(filtered);
    

    array_destroy(arr);
}

TEST(OperationsTest, MapTransform) {
    DynArray* arr = array_create(element_info_student());
    array_push(arr, student_create("Test", 20, 10, 4.0));

    DynArray* mapped = array_map(arr, [](void* p) -> void* {
        if(!p) return nullptr;
        ((Person*)p)->id += 100;
        return p;
    });
    
    ASSERT_NE(mapped, nullptr);
    Student* res = static_cast<Student*>(array_get(mapped, 0));
    ASSERT_NE(res, nullptr);
    EXPECT_EQ(res->base.id, 110);


    free(mapped->data);
    free(mapped);
    
    array_destroy(arr);
}
