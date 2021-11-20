cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 11)
project(beans)

file(GLOB_RECURSE SOURCES src/*)

add_executable(beans ${SOURCES})