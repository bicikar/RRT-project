cmake_minimum_required(VERSION 3.20)
project(RRT_linux)

set(CMAKE_CXX_STANDARD 17)

add_executable(RRT_linux src/main.cpp src/main.cpp include/field.h src/field.cpp include/kdtree.h include/tree.h)
