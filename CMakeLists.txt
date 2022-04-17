cmake_minimum_required(VERSION 3.21)
project(lcf_json C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(lcf_json lcf_json.c lcf_json.h)
add_executable(test test.c)
target_link_libraries(test lcf_json)
