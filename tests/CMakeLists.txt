add_executable(run_tests
    test_dynarray.cpp
    test_polymorphism.cpp
    test_operations.cpp
)

target_link_libraries(run_tests PRIVATE dynarray_static gtest_main)

if(MSVC)
    target_compile_definitions(run_tests PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
