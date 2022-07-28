cmake_minimum_required(VERSION 3.21)
project(IMGUI)

file(GLOB IMGUI_SRC RELATIVE "${CMAKE_SOURCE_DIR}/lib/imgui" "*.cpp")
file(GLOB_RECURSE IMGUI_HEADER RELATIVE "${CMAKE_SOURCE_DIR}/lib/imgui" "../imgui/*.h")
string(REPLACE "../../imgui/imgui_impl_win32.cpp" "" out ${IMGUI_SRC})

add_library(IMGUI OBJECT ${IMGUI_SRC})
target_compile_definitions(IMGUI PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_include_directories(IMGUI PUBLIC ".")
target_link_libraries(IMGUI glfw)
