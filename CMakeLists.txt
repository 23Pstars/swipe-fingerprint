cmake_minimum_required(VERSION 3.6)
project(swipe_fingerprint)

set(CMAKE_CXX_STANDARD 11)

set(HEADER_FILES config.h bmp.h corr.h)

set(SOURCE_FILES main.c bmp.c corr.c)
#set(SOURCE_FILES info.c bmp.c)
#set(SOURCE_FILES test.c corr.c bmp.c)
#set(SOURCE_FILES split.c corr.c bmp.c)
#set(SOURCE_FILES gmfinger.c corr.c bmp.c)

add_executable(swipe_fingerprint ${HEADER_FILES} ${SOURCE_FILES})