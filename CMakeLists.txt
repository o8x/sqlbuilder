cmake_minimum_required(VERSION 3.31)
project(sqlbuilder)

set(CMAKE_CXX_STANDARD 20)

add_library(sqlbuilder SHARED sqlbuilder.h sqlbuilder.cpp helpers.cpp helpers.h column.cpp column.h)
target_include_directories(sqlbuilder PUBLIC .)

add_subdirectory(example)
