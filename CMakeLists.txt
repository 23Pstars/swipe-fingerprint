cmake_minimum_required(VERSION 3.6)
project(BMP)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#set(SOURCE_FILES main.c bmp.c)
set(SOURCE_FILES bmp8bit.c)

add_executable(BMP ${SOURCE_FILES})