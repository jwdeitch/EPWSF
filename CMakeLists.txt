cmake_minimum_required(VERSION 3.6)
project(lss)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES lss.c)
add_executable(lss ${SOURCE_FILES})