cmake_minimum_required(VERSION 3.16)

project(cpp_hw8)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER "/usr/bin/g++-10")


include_directories(.)

add_executable(8_1 8_1.cpp 8_1.h)
target_link_libraries(8_1 PRIVATE atomic)
add_executable(8_2
        8_2.cpp
        8_2.h)
