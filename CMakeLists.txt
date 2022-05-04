cmake_minimum_required(VERSION 3.22)
project(lab_3_files_OS C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(lab_3_files_OS
        caltrain-runner.c
        caltrain.c
        caltrain.h)

set(GCC_COVERAGE_COMPILE_FLAGS "-pthread")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
