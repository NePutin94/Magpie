cmake_minimum_required(VERSION 3.21)
project(IMGUI)
if (UNIX)
    SET(IMGUI_SRC imgui.cpp imgui_demo.cpp imgui_draw.cpp
            imgui_impl_glfw.cpp imgui_impl_opengl3.cpp imgui_tables.cpp imgui_widgets.cpp implot.cpp
            implot_demo.cpp implot_internal.h implot_items.cpp)
endif ()
if (WIN32)
    SET(IMGUI_SRC imgui.cpp imgui_demo.cpp imgui_draw.cpp imgui_impl_win32.cpp
            imgui_impl_glfw.cpp imgui_impl_opengl3.cpp imgui_tables.cpp imgui_widgets.cpp implot.cpp
            implot_demo.cpp implot_internal.h implot_items.cpp imgui.h imgui_impl_glfw.h imgui_impl_opengl3.h imgui_impl_win32.h imgui_internal.h implot.h implot_internal.h)
endif ()

#file(GLOB IMGUI_SRC RELATIVE "${CMAKE_SOURCE_DIR}/lib/imgui" "*.cpp")
#tring(REPLACE "../../imgui/imgui_impl_win32.cpp" "" out ${IMGUI_SRC})

add_library(IMGUI OBJECT ${IMGUI_SRC})
target_compile_definitions(IMGUI PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_include_directories(IMGUI PUBLIC ".")
target_link_libraries(IMGUI glfw)
