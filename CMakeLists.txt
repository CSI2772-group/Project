cmake_minimum_required(VERSION 3.15)
project(lab-5)
set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE SOURCES src/*)

add_executable(beans ${SOURCES})