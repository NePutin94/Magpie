cmake_minimum_required(VERSION 3.21)
#project(IMGUI)
#if (UNIX)
#    SET(IMGUI_SRC imgui.cpp imgui_demo.cpp imgui_draw.cpp
#            imgui_impl_glfw.cpp imgui_impl_opengl3.cpp imgui_tables.cpp imgui_widgets.cpp implot.cpp
#            implot_demo.cpp implot_internal.h implot_items.cpp)
#endif ()
#if (WIN32)
#    SET(IMGUI_SRC imgui.cpp imgui_demo.cpp imgui_draw.cpp imgui_impl_win32.cpp
#            imgui_impl_glfw.cpp imgui_impl_opengl3.cpp imgui_tables.cpp imgui_widgets.cpp implot.cpp
#            implot_demo.cpp implot_internal.h implot_items.cpp imgui.h imgui_impl_glfw.h imgui_impl_opengl3.h imgui_impl_win32.h imgui_internal.h implot.h implot_internal.h)
#endif ()
#
##file(GLOB IMGUI_SRC RELATIVE "${CMAKE_SOURCE_DIR}/lib/imgui" "*.cpp")
##tring(REPLACE "../../imgui/imgui_impl_win32.cpp" "" out ${IMGUI_SRC})
#
#add_library(IMGUI OBJECT ${IMGUI_SRC})
#target_compile_definitions(IMGUI PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
#target_include_directories(IMGUI PUBLIC ".")
#target_link_libraries(IMGUI glfw)
project(imgui)
add_library(${PROJECT_NAME} "")
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(
        ${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_sources(
        ${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui_demo.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui_draw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui_tables.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui_widgets.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui_stdlib.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/implot.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/implot_items.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/implot_demo.cpp
)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/imgui_impl_glfw.cpp)
target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/imgui_impl_opengl3.cpp)
