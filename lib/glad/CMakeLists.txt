cmake_minimum_required(VERSION 3.19)
project(GLAD)

file(GLOB_RECURSE GLAD_SRC RELATIVE "${CMAKE_SOURCE_DIR}/lib/glad" "../glad/src/*.c")
set(GLAD_INCLUDE "include")

add_library(GLAD OBJECT ${GLAD_SRC})
target_include_directories(GLAD PUBLIC ${GLAD_INCLUDE})
