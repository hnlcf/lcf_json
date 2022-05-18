cmake_minimum_required(VERSION 3.21)
project(lcf_json C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(
        LIB_SOURCE_FILE
        src/lcf_json.c
)
add_library(${PROJECT_NAME} ${LIB_SOURCE_FILE})

include_directories(
        include
)
set(
        TEST_SOURCE_FILE
        src/test.c
)
add_executable("${PROJECT_NAME}_test" ${TEST_SOURCE_FILE})
target_link_libraries("${PROJECT_NAME}_test" lcf_json)
