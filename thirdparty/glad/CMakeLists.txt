cmake_minimum_required(VERSION 3.16)
project(glad LANGUAGES C)

add_library(glad STATIC src/gl.c)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
