#cmake_minimum_required(VERSION 3.21)
#project(Magpie)
#
#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#
#set(CMAKE_COLOR_DIAGNOSTICS ON)
#set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
#if (TracyProfiler AND NOT MSVC)
#    message(FATAL_ERROR "Tracy profiler can only be used with the msvc compiler")
#endif ()
#
#if (MSVC)
#    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
#    set(BUILD_SHARED_LIBS TRUE)
#
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -DDEBUG ")
#    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG -DTracyProfiler /Ox /Ob2 /Oi /Ot /Zi /OPT:REF /GF /MT ")
#else ()
#    if (WIN32)
#        if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
#            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -DDEBUG")
#        else ()
#            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DDEBUG -mwindows -s")
#        endif ()
#    ENDIF ()
#endif ()
#
#IF (UNIX)
#    if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
#        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -DDEBUG")
#    else ()
#        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG -s")
#    endif ()
#    set(CMAKE_THREAD_LIBS_INIT "-lpthread")
#    set(CMAKE_HAVE_THREADS_LIBRARY 1)
#    set(CMAKE_USE_WIN32_THREADS_INIT 0)
#    set(CMAKE_USE_PTHREADS_INIT 1)
#    set(THREADS_PREFER_PTHREAD_FLAG ON)
#    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
#ENDIF ()
#
#
#file(GLOB Magpie_SRC RELATIVE "${CMAKE_SOURCE_DIR}/" "src/*.cpp")
#file(GLOB IMGUI_SRC RELATIVE "${CMAKE_SOURCE_DIR}/" "lib/imgui/*.cpp")
#if (NOT MSVC)
#    add_executable(Magpie src/main.cpp ${Magpie_SRC} include/config.h include/DataSets.h)
#else ()
#    add_executable(Magpie src/main.cpp ${Magpie_SRC} ${IMGUI_SRC} include/config.h include/DataSets.h)
#endif ()
#if (MSVC)
#    set_target_properties(Magpie PROPERTIES
#            LINK_FLAGS /SUBSYSTEM:CONSOLE
#            )
#endif ()
#add_subdirectory(lib)
#if (NOT MSVC)
#    add_subdirectory(tests)
#endif ()
#FetchContent_Declare(
#        tracy
#        GIT_REPOSITORY https://github.com/wolfpld/tracy.git
#        GIT_TAG "v0.9"
#)
#set(TRACY_NO_VSYNC_CAPTURE ON)
#FetchContent_MakeAvailable(tracy)
#
#FetchContent_Declare(nlohmannjson
#        GIT_REPOSITORY https://github.com/nlohmann/json.git
#        GIT_TAG "v3.11.2")
#set(JSON_BuildTests OFF)
#FetchContent_MakeAvailable(nlohmannjson)
#
#target_include_directories(Magpie PUBLIC lib/palka_core)
#target_link_libraries(Magpie PUBLIC palka TracyClient nlohmann_json::nlohmann_json)
#
#if (NOT MSVC)
#    add_library(MagpieLib SHARED ${Magpie_SRC})
#    target_include_directories(MagpieLib PUBLIC lib/palka_core)
#    target_link_libraries(MagpieLib PUBLIC palka TracyClient nlohmann_json::nlohmann_json)
#    target_include_directories(MagpieLib PUBLIC .)
#endif ()
#
#add_custom_command(TARGET Magpie POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:palka> $<TARGET_FILE_DIR:Magpie>
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:RTTR::Core> $<TARGET_FILE_DIR:Magpie>
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:glfw> $<TARGET_FILE_DIR:Magpie>
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:assimp::assimp> $<TARGET_FILE_DIR:Magpie>
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:tinygltf> $<TARGET_FILE_DIR:Magpie>
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:fmt::fmt> $<TARGET_FILE_DIR:Magpie>
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:TracyClient> $<TARGET_FILE_DIR:Magpie>
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:imgui::imgui> $<TARGET_FILE_DIR:Magpie>
#
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:palka> "${CMAKE_BINARY_DIR}/tests"
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:fmt::fmt> "${CMAKE_BINARY_DIR}/tests"
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:assimp::assimp> "${CMAKE_BINARY_DIR}/tests"
#
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:RTTR::Core> "${CMAKE_BINARY_DIR}/tests"
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:glfw> "${CMAKE_BINARY_DIR}/tests"
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:tinygltf> "${CMAKE_BINARY_DIR}/tests")
#
#if (NOT MSVC)
#    add_custom_command(TARGET Magpie POST_BUILD
#            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:MagpieLib> "${CMAKE_BINARY_DIR}/tests")
#endif ()
#


cmake_minimum_required(VERSION 3.21)
project(Magpie)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
if (TracyProfiler AND NOT MSVC)
    message(FATAL_ERROR "Tracy profiler can only be used with the msvc compiler")
endif ()

if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set(BUILD_SHARED_LIBS TRUE)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -DDEBUG ")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG -DTracyProfiler /Ox /Ob2 /Oi /Ot /Zi /OPT:REF /GF /MT ")
else ()
    if (WIN32)
        add_compile_options(-Wa,-mbig-obj)
        if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -DDEBUG")
        else ()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DDEBUG -mwindows -s")
        endif ()
    ENDIF ()
endif ()

IF (UNIX)
    if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -DDEBUG")
    else ()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG -s")
    endif ()
    set(CMAKE_THREAD_LIBS_INIT "-lpthread")
    set(CMAKE_HAVE_THREADS_LIBRARY 1)
    set(CMAKE_USE_WIN32_THREADS_INIT 0)
    set(CMAKE_USE_PTHREADS_INIT 1)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
ENDIF ()


file(GLOB Magpie_SRC RELATIVE "${CMAKE_SOURCE_DIR}/" "src/*.cpp")
file(GLOB IMGUI_SRC RELATIVE "${CMAKE_SOURCE_DIR}/" "lib/imgui/*.cpp")



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

FetchContent_Declare(
        tracy
        GIT_REPOSITORY https://github.com/wolfpld/tracy.git
        GIT_TAG "v0.9"
)
set(TRACY_NO_VSYNC_CAPTURE ON)
FetchContent_MakeAvailable(tracy)

FetchContent_Declare(nlohmannjson
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG "v3.11.2")
set(JSON_BuildTests OFF)
FetchContent_MakeAvailable(nlohmannjson)


add_subdirectory(lib/glad)
#add_subdirectory(imgui_glfw)
add_subdirectory(lib/imgui)
add_subdirectory(lib/stb)
#add_subdirectory(palka_core)

#add_library(palka SHARED PalkaCore)
#target_link_libraries(palka PUBLIC PalkaCore)

file(GLOB CORE_SRC RELATIVE "${CMAKE_SOURCE_DIR}/" "lib/palka_core/*.cpp")
file(GLOB_RECURSE GLAD_SRC RELATIVE "${CMAKE_SOURCE_DIR}/" "lib/glad/src/*.c")
set(HEADER_SOURCE lib/palka_core/ReflectionDebug.h lib/palka_core/Drawable.h)
set(GLAD_INCLUDE "lib/glad/include")
find_package(OpenGL REQUIRED)
#IMGUI_GLFW
message(STATUS ${Magpie_SRC})
message(STATUS ${CORE_SRC})
if (WIN32)
    file(GLOB IMGUI_SRC RELATIVE "${CMAKE_SOURCE_DIR}/" "lib/imgui/*.cpp")
endif ()


add_executable(Magpie src/main.cpp ${Magpie_SRC} ${CORE_SRC} ${HEADER_SOURCE} ${IMGUI_SRC} include/config.h include/DataSets.h)
target_link_libraries(Magpie PUBLIC ${OPENGL_LIBRARIES} TracyClient nlohmann_json::nlohmann_json tinygltf EnTT::EnTT assimp::assimp imgui::imgui fmt::fmt glm::glm glfw RTTR::Core GLAD STB imm32 ws2_32 dwmapi)
target_compile_definitions(Magpie PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD GL_GLEXT_PROTOTYPES=1)
target_include_directories(Magpie PUBLIC ${OPENGL_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ../lib/imgui)
include_directories(lib/palka_core)

add_custom_command(TARGET Magpie POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:RTTR::Core> $<TARGET_FILE_DIR:Magpie>
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:glfw> $<TARGET_FILE_DIR:Magpie>
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:assimp::assimp> $<TARGET_FILE_DIR:Magpie>
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:tinygltf> $<TARGET_FILE_DIR:Magpie>
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:fmt::fmt> $<TARGET_FILE_DIR:Magpie>
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:TracyClient> $<TARGET_FILE_DIR:Magpie>
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:imgui::imgui> $<TARGET_FILE_DIR:Magpie>)


#
#if (NOT MSVC)
#    add_executable(Magpie src/main.cpp ${Magpie_SRC} include/config.h include/DataSets.h)
#else ()
#    add_executable(Magpie src/main.cpp ${Magpie_SRC} ${IMGUI_SRC} include/config.h include/DataSets.h)
#endif ()
#if (MSVC)
#    set_target_properties(Magpie PROPERTIES
#            LINK_FLAGS /SUBSYSTEM:CONSOLE
#            )
#endif ()
#add_subdirectory(lib)
#if (NOT MSVC)
#    add_subdirectory(tests)
#endif ()
#FetchContent_Declare(
#        tracy
#        GIT_REPOSITORY https://github.com/wolfpld/tracy.git
#        GIT_TAG "v0.9"
#)
#set(TRACY_NO_VSYNC_CAPTURE ON)
#FetchContent_MakeAvailable(tracy)
#
#FetchContent_Declare(nlohmannjson
#        GIT_REPOSITORY https://github.com/nlohmann/json.git
#        GIT_TAG "v3.11.2")
#set(JSON_BuildTests OFF)
#FetchContent_MakeAvailable(nlohmannjson)
#
#target_include_directories(Magpie PUBLIC lib/palka_core)
#target_link_libraries(Magpie PUBLIC palka TracyClient nlohmann_json::nlohmann_json)
#
#if (NOT MSVC)
#    add_library(MagpieLib SHARED ${Magpie_SRC})
#    target_include_directories(MagpieLib PUBLIC lib/palka_core)
#    target_link_libraries(MagpieLib PUBLIC palka TracyClient nlohmann_json::nlohmann_json)
#    target_include_directories(MagpieLib PUBLIC .)
#endif ()
#
#add_custom_command(TARGET Magpie POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:palka> $<TARGET_FILE_DIR:Magpie>
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:RTTR::Core> $<TARGET_FILE_DIR:Magpie>
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:glfw> $<TARGET_FILE_DIR:Magpie>
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:assimp::assimp> $<TARGET_FILE_DIR:Magpie>
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:tinygltf> $<TARGET_FILE_DIR:Magpie>
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:fmt::fmt> $<TARGET_FILE_DIR:Magpie>
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:TracyClient> $<TARGET_FILE_DIR:Magpie>
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:imgui::imgui> $<TARGET_FILE_DIR:Magpie>
#
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:palka> "${CMAKE_BINARY_DIR}/tests"
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:fmt::fmt> "${CMAKE_BINARY_DIR}/tests"
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:assimp::assimp> "${CMAKE_BINARY_DIR}/tests"
#
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:RTTR::Core> "${CMAKE_BINARY_DIR}/tests"
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:glfw> "${CMAKE_BINARY_DIR}/tests"
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:tinygltf> "${CMAKE_BINARY_DIR}/tests")
#
#if (NOT MSVC)
#    add_custom_command(TARGET Magpie POST_BUILD
#            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:MagpieLib> "${CMAKE_BINARY_DIR}/tests")
#endif ()



