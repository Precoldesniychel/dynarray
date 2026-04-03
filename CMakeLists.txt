cmake_minimum_required(VERSION 3.10)
project(Variant17 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")

if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

include_directories(include)

add_library(core STATIC
    src/person.c
    src/student.c
    src/teacher.c
    src/element_info.c
    src/dynarray.c
    src/operations.c
)

add_executable(v17 
    main.c
    cli/interface.c
)
target_link_libraries(v17 core)

add_executable(tests tests/test_all.c)
target_link_libraries(tests core)

if(WIN32)
    add_custom_command(TARGET tests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:tests>
        $<TARGET_FILE_DIR:v17>/tests.exe)
else()
    add_custom_command(TARGET tests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:tests>
        $<TARGET_FILE_DIR:v17>/tests)
endif()

add_custom_target(run COMMAND v17 DEPENDS v17)
add_custom_target(test COMMAND tests DEPENDS tests)