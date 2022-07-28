cmake_minimum_required(VERSION 3.21)
project(Magpie)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-std=c++2a")

if (WIN32)
    if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
    else ()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG -mwindows -s")
    endif ()
ENDIF ()

IF (UNIX)
    if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
    else ()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG -s")
    endif ()
    set(CMAKE_THREAD_LIBS_INIT "-lpthread")
    set(CMAKE_HAVE_THREADS_LIBRARY 1)
    set(CMAKE_USE_WIN32_THREADS_INIT 0)
    set(CMAKE_USE_PTHREADS_INIT 1)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
ENDIF ()

file(GLOB Billet_SRC RELATIVE "${CMAKE_SOURCE_DIR}/" "*.cpp")
add_executable(Magpie main.cpp ${Billet_SRC})

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
add_subdirectory(lib)
target_include_directories(Magpie PUBLIC lib/palka_core)
target_link_libraries(Magpie palka)

add_custom_command(TARGET Magpie POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy  $<TARGET_FILE:palka> $<TARGET_FILE_DIR:Magpie>
        COMMAND ${CMAKE_COMMAND} -E copy  $<TARGET_FILE:RTTR::Core> $<TARGET_FILE_DIR:Magpie>
        COMMAND ${CMAKE_COMMAND} -E copy  $<TARGET_FILE:glfw> $<TARGET_FILE_DIR:Magpie>
        COMMAND ${CMAKE_COMMAND} -E copy  $<TARGET_FILE:assimp::assimp> $<TARGET_FILE_DIR:Magpie>
        COMMAND ${CMAKE_COMMAND} -E copy  $<TARGET_FILE:tinygltf> $<TARGET_FILE_DIR:Magpie>
        COMMAND ${CMAKE_COMMAND} -E copy  $<TARGET_FILE:fmt::fmt> $<TARGET_FILE_DIR:Magpie>)
