cmake_minimum_required(VERSION 3.6)
project(c-bmp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c bmp.c corr.c)
#set(SOURCE_FILES info.c bmp.c)
#set(SOURCE_FILES bmp8bit.c)
#set(SOURCE_FILES tests.c bmp.c corr.c)
#set(SOURCE_FILES tests_2d.c corr_2d.c)

add_executable(c-bmp ${SOURCE_FILES})