cmake_minimum_required(VERSION 3.18.1)

project("opencldemo")

include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/include)
include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/include/CL)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI})

add_library(
        opencldemo

        SHARED

        src/main/cpp/opencl_wrapper.cpp
        src/main/cpp/opencl_add_kernel.cl)

find_library(
        log-lib

        log)

target_link_libraries(
        opencldemo

        ${log-lib})