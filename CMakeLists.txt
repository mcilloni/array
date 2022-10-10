cmake_minimum_required(VERSION 3.16.0)

project(array VERSION 0.0.1 LANGUAGES C)

file(GLOB common_SRC
    "include/*.h"
    "src/*.c"
)

set(CMAKE_C_STANDARD 17)

set(CMAKE_C_STANDARD_REQUIRED ON)

if (NOT MSVC)
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_library(array STATIC ${common_SRC})

target_include_directories (
    array PUBLIC 
    "${PROJECT_SOURCE_DIR}/include"
)
    
add_executable(ex examples/ex.c)
target_link_libraries(ex array)

add_executable(allocex examples/allocex.c)
target_link_libraries(allocex array)

add_executable(custallex examples/custallex.c)
target_link_libraries(custallex array)

add_executable(slicex examples/slicex.c)
target_link_libraries(slicex array)

add_executable(strex examples/strex.c)
target_link_libraries(strex array)