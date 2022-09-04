cmake_minimum_required(VERSION 3.21)
project(Magpie)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_COLOR_DIAGNOSTICS ON)

if (TracyProfiler AND NOT MSVC)
    message(FATAL_ERROR "Tracy profiler can only be used with the msvc compiler")
endif ()

if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set(BUILD_SHARED_LIBS TRUE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG -DTracyProfiler /Ox /Ob2 /Oi /Ot /Zi /OPT:REF /GF /MT ")
else ()
    if (WIN32)
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


file(GLOB Magpie_SRC RELATIVE "${CMAKE_SOURCE_DIR}/" "*.cpp")
file(GLOB IMGUI_SRC RELATIVE "${CMAKE_SOURCE_DIR}/" "lib/imgui/*.cpp")
if (NOT MSVC)
    add_executable(Magpie main.cpp ${Magpie_SRC})
else ()
    add_executable(Magpie main.cpp ${Magpie_SRC} ${IMGUI_SRC})
endif ()
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
add_subdirectory(lib)
if (NOT MSVC)
    add_subdirectory(tests)
endif ()
FetchContent_Declare(
        tracy
        GIT_REPOSITORY https://github.com/wolfpld/tracy.git
        GIT_TAG "v0.8.2"
)
set(TRACY_NO_VSYNC_CAPTURE ON)
FetchContent_MakeAvailable(tracy)

target_include_directories(Magpie PUBLIC lib/palka_core)
target_link_libraries(Magpie PUBLIC palka TracyClient)

if (NOT MSVC)
    add_library(MagpieLib SHARED ${Magpie_SRC})
    target_include_directories(MagpieLib PUBLIC lib/palka_core)
    target_link_libraries(MagpieLib PUBLIC palka TracyClient)
    target_include_directories(MagpieLib PUBLIC .)
endif ()
add_custom_command(TARGET Magpie POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:palka> $<TARGET_FILE_DIR:Magpie>
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:RTTR::Core> $<TARGET_FILE_DIR:Magpie>
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:glfw> $<TARGET_FILE_DIR:Magpie>
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:assimp::assimp> $<TARGET_FILE_DIR:Magpie>
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:tinygltf> $<TARGET_FILE_DIR:Magpie>
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:fmt::fmt> $<TARGET_FILE_DIR:Magpie>
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:TracyClient> $<TARGET_FILE_DIR:Magpie>

        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:palka> "${CMAKE_BINARY_DIR}/tests"
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:fmt::fmt> "${CMAKE_BINARY_DIR}/tests"
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:assimp::assimp> "${CMAKE_BINARY_DIR}/tests"
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:MagpieLib> "${CMAKE_BINARY_DIR}/tests"
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:RTTR::Core> "${CMAKE_BINARY_DIR}/tests"
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:glfw> "${CMAKE_BINARY_DIR}/tests"
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:tinygltf> "${CMAKE_BINARY_DIR}/tests")


