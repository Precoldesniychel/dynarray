cmake_minimum_required(VERSION 3.14)
project(DynArrayLab C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

# 1. Статическая библиотека
add_library(dynarray_static STATIC
    src/dynarray.c src/element_info.c src/person.c
    src/student.c src/teacher.c src/operations.c
)

# 2. Основная программа
add_executable(v17 main.c cli/interface.c)
target_link_libraries(v17 PRIVATE dynarray_static)
if(MSVC)
    target_compile_definitions(v17 PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

# 3. Google Test (ОБЯЗАТЕЛЬНО ДО add_subdirectory(tests)!)
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
    GIT_SHALLOW    TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# 4. Тесты
add_subdirectory(tests)