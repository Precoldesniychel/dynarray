#include <gtest/gtest.h>
#include "dynarray.h"
#include "element_info.h"

TEST(DynArrayTest, CreateAndDestroy) {
    DynArray* arr = array_create(element_info_int());
    ASSERT_NE(arr, nullptr);
    EXPECT_EQ(array_size(arr), 0);
    EXPECT_EQ(array_capacity(arr), 0);
    array_destroy(arr);
}

TEST(DynArrayTest, PushAndGet) {
    DynArray* arr = array_create(element_info_int());
    int* val1 = new int(10);
    int* val2 = new int(20);
    
    EXPECT_EQ(array_push(arr, val1), 0);
    EXPECT_EQ(array_push(arr, val2), 0);
    
    EXPECT_EQ(array_size(arr), 2);
    EXPECT_GT(array_capacity(arr), 0); 
    EXPECT_EQ(*static_cast<int*>(array_get(arr, 0)), 10);
    EXPECT_EQ(*static_cast<int*>(array_get(arr, 1)), 20);
    
    delete val1; 
    delete val2;
    array_destroy(arr);
}

TEST(DynArrayTest, BoundaryChecks) {
    DynArray* arr = array_create(element_info_double());
    EXPECT_EQ(array_get(arr, 0), nullptr); 
    EXPECT_EQ(array_remove_at(arr, 5), -1); 
    array_destroy(arr);
}