cmake_minimum_required(VERSION 3.21)
project(palka)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set(BUILD_SHARED_LIBS TRUE)
    #    if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    #        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG /DEBUG /INCREMENTAL:NO /O1 ")
    #    else ()
    #        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG /Ox /Ob2 /Oi /Ot /Zi /OPT:REF /GF /MT ")
    #    endif ()
else ()
    if (WIN32)
        if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
        else ()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG -mwindows -s")
        endif ()
    ENDIF ()
endif ()
IF (UNIX)
    if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
    else ()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG -s ")
    endif ()
    set(CMAKE_THREAD_LIBS_INIT "-lpthread")
    set(CMAKE_HAVE_THREADS_LIBRARY 1)
    set(CMAKE_USE_WIN32_THREADS_INIT 0)
    set(CMAKE_USE_PTHREADS_INIT 1)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
ENDIF ()

include(FetchContent)

set(BUILD_UNIT_TESTS OFF)
set(BUILD_STATIC OFF)
set(BUILD_BENCHMARKS OFF)
set(BUILD_EXAMPLES OFF)
set(BUILD_DOCUMENTATION OFF)
FetchContent_Declare(
        rttr
        GIT_REPOSITORY https://github.com/rttrorg/rttr.git
        # GIT_TAG 7edbd580cfad509a3253c733e70144e36f02ecd4
)
set(BUILD_RTTR_DYNAMIC ON)
set(BUILD_UNIT_TESTS OFF)
set(BUILD_STATIC OFF)
set(BUILD_EXAMPLES OFF)
set(BUILD_DOCUMENTATION OFF)
FetchContent_MakeAvailable(rttr)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG "3.3.5"
)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(BUILD_SHARED_LIBS ON)
FetchContent_MakeAvailable(glfw)


set(FMT_DOC OFF)
set(FMT_TEST OFF)
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        # GIT_TAG 7edbd580cfad509a3253c733e70144e36f02ecd4
)
FetchContent_MakeAvailable(fmt)


FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG "0.9.9.8"
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
        entt
        GIT_REPOSITORY https://github.com/skypjack/entt.git
        GIT_TAG "v3.8.1"
)
FetchContent_MakeAvailable(entt)

set(TINYGLTF_BUILD_LOADER_EXAMPLE OFF)
FetchContent_Declare(
        tinygltf
        GIT_REPOSITORY https://github.com/syoyo/tinygltf.git
        GIT_TAG "v2.6.3"
)
FetchContent_MakeAvailable(tinygltf)

FetchContent_Declare(
        assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG "v5.1.3"
)
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_BUILD_ZLIB ON)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF)
set(ASSIMP_BUILD_OBJ_IMPORTER ON)
set(ASSIMP_BUILD_FBX_IMPORTER ON)
FetchContent_MakeAvailable(assimp)


add_subdirectory(glad)
#add_subdirectory(imgui_glfw)
add_subdirectory(imgui)
add_subdirectory(stb)
#add_subdirectory(palka_core)

#add_library(palka SHARED PalkaCore)
#target_link_libraries(palka PUBLIC PalkaCore)

file(GLOB CORE_SRC RELATIVE "${CMAKE_SOURCE_DIR}/lib" "../lib/palka_core/*.cpp")
file(GLOB_RECURSE GLAD_SRC RELATIVE "${CMAKE_SOURCE_DIR}/lib/" "../lib/glad/src/*.c")
set(HEADER_SOURCE palka_core/ReflectionDebug.h palka_core/Drawable.h)
set(GLAD_INCLUDE "../glad/include")
find_package(OpenGL REQUIRED)
#IMGUI_GLFW
if (UNIX)
    SET(IMGUI_SRC imgui/imgui.cpp imgui/imgui_demo.cpp imgui/imgui_draw.cpp
            imgui/imgui_impl_glfw.cpp imgui/imgui_impl_opengl3.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp imgui/implot.cpp
            imgui/implot_demo.cpp imgui/implot_internal.h imgui/implot_items.cpp)
endif ()
if (WIN32)
    SET(IMGUI_SRC imgui/imgui.cpp imgui/imgui_demo.cpp imgui/imgui_draw.cpp imgui/imgui_impl_win32.cpp
            imgui/imgui_impl_glfw.cpp imgui/imgui_impl_opengl3.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp imgui/implot.cpp
            imgui/implot_demo.cpp imgui/implot_internal.h imgui/implot_items.cpp imgui/imgui.himgui/ imgui_impl_glfw.h
            imgui/imgui_impl_opengl3.h imgui/imgui_impl_win32.h imgui/imgui_internal.h imgui/implot.h imgui/implot_internal.h)
endif ()
if (NOT MSVC)
    add_library(palka SHARED ${CORE_SRC} ${HEADER_SOURCE})
else ()
    add_library(palka SHARED ${CORE_SRC} ${HEADER_SOURCE} ${IMGUI_SRC})
endif ()

if (WIN32)
    if (NOT MSVC)
        target_link_libraries(palka PUBLIC ${OPENGL_LIBRARIES} tinygltf EnTT::EnTT assimp::assimp fmt::fmt glm::glm glfw RTTR::Core STB GLAD imgui::imgui imm32 ws2_32 dwmapi)
    else ()
        target_link_libraries(palka PUBLIC ${OPENGL_LIBRARIES} tinygltf EnTT::EnTT assimp::assimp imgui::imgui fmt::fmt glm::glm glfw RTTR::Core GLAD STB imm32 ws2_32 dwmapi)
    endif ()
endif ()
IF (UNIX)
    target_link_libraries(palka PUBLIC ${OPENGL_LIBRARIES} tinygltf EnTT::EnTT assimp::assimp fmt::fmt glm::glm glfw RTTR::Core STB imgui::imgui GLAD)
endif ()
target_compile_definitions(palka PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD GL_GLEXT_PROTOTYPES=1)
target_include_directories(palka PUBLIC ${OPENGL_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ../lib/imgui)


#add_custom_command(TARGET palka POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy
#        $<TARGET_FILE_DIR:palka>
#        "${PROJECT_SOURCE_DIR}/cmake-build-debug")

#install(TARGETS palka DESTINATION "${PROJECT_SOURCE_DIR}/cmake-build-debug")