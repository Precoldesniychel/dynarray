add_executable(run_tests
    test_dynarray.cpp
    test_polymorphism.cpp
    test_operations.cpp
)

target_link_libraries(run_tests PRIVATE 
    dynarray_static 
    gtest_main
)