cmake_minimum_required(VERSION 3.20)
project(Tests)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE HEADERS RELATIVE "${CMAKE_SOURCE_DIR}/tests" "../include/*.h")
file(GLOB_RECURSE SOURCES RELATIVE "${CMAKE_SOURCE_DIR}/tests" "../src/*.cpp")

include(FetchContent)
FetchContent_Declare(
        gtest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(gtest)

enable_testing()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTESTING")
include(GoogleTest)
add_executable(Tests WIN32 tests.cpp TestFractus.cpp TestPlotHasPoint.cpp TestGraphicMethod.cpp TestPlotIntersection.cpp TestGraphicMethodSort2.cpp TestGraphicMethodSolve.cpp TestMatrixStorage.cpp)
target_link_libraries(Tests GTest::gtest_main MagpieLib)
gtest_discover_tests(Tests)

add_custom_command(TARGET Tests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy  $<TARGET_FILE:palka>  $<TARGET_FILE_DIR:Tests>
        COMMAND ${CMAKE_COMMAND} -E copy  $<TARGET_FILE:RTTR::Core>  $<TARGET_FILE_DIR:Tests>
        COMMAND ${CMAKE_COMMAND} -E copy  $<TARGET_FILE:glfw>  $<TARGET_FILE_DIR:Tests>
        COMMAND ${CMAKE_COMMAND} -E copy  $<TARGET_FILE:assimp::assimp>  $<TARGET_FILE_DIR:Tests>
#        COMMAND ${CMAKE_COMMAND} -E copy  $<TARGET_FILE:tinygltf>  $<TARGET_FILE_DIR:Tests>
        COMMAND ${CMAKE_COMMAND} -E copy  $<TARGET_FILE:fmt::fmt>  $<TARGET_FILE_DIR:Tests>)

