cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 11)
project(beans)

file(GLOB_RECURSE SOURCES src/*)

if (MSVC)
    # warning level 4
    add_compile_options(/W4)
else ()
    # Show a bunch of warnings :)
    add_compile_options(-Wall -Wextra -pedantic)
endif ()

add_executable(beans ${SOURCES})