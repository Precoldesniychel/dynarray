cmake_minimum_required(VERSION 3.14)
project(DynArrayLab C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Явно задаём пути для исполняемых файлов (избегает путаницы в VS)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)

if(MSVC)
    add_compile_options(/W4)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

# Библиотека (C)
add_library(dynarray_static STATIC
    src/dynarray.c
    src/element_info.c
    src/person.c
    src/student.c
    src/teacher.c
)

# Консольная оболочка (C)
add_executable(v17 main.c cli/interface.c)
target_link_libraries(v17 PRIVATE dynarray_static)

# Google Test (C++)
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
    GIT_SHALLOW    TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory(tests)