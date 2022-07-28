cmake_minimum_required(VERSION 3.19)

project(PalkaCore)

file(GLOB CORE_SRC RELATIVE "${CMAKE_SOURCE_DIR}/lib/palka_core" "../palka_core/*.cpp")
file(GLOB GLAD_SRC RELATIVE "${CMAKE_SOURCE_DIR}/lib/palka_core" "../glad/src/*.c")
set(HEADER_SOURCE ReflectionDebug.h Drawable.h)
set(GLAD_INCLUDE "../glad/include")
find_package(OpenGL REQUIRED)
if (WIN32)
    add_library(PalkaCore SHARED ${CORE_SRC} ${HEADER_SOURCE})
    target_link_libraries(PalkaCore PUBLIC mingw32 ${OPENGL_LIBRARIES} tinygltf EnTT::EnTT assimp::assimp fmt::fmt glm::glm glfw RTTR::Core STB IMGUI_GLFW IMGUI GLAD imm32 ws2_32 dwmapi)
    target_compile_definitions(PalkaCore PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD GL_GLEXT_PROTOTYPES=1)
    target_include_directories(PalkaCore PUBLIC ${OPENGL_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})
endif (WIN32)

IF (UNIX)
    add_library(PalkaCore STATIC ${CORE_SRC} ${HEADER_SOURCE})
    target_link_libraries(PalkaCore PUBLIC mingw32 ${OPENGL_LIBRARIES} assimp::assimp fmt::fmt glm::glm glfw RTTR::Core STB IMGUI_GLFW IMGUI GLAD imm32 ws2_32 dwmapi)
    target_compile_definitions(PalkaCore PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD GL_GLEXT_PROTOTYPES=1)
    target_include_directories(PalkaCore PUBLIC ${OPENGL_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})
endif (UNIX)
#cmake_minimum_required(VERSION 3.19)
#
#project(PalkaCore)
#
#file(GLOB CORE_SRC RELATIVE "${CMAKE_SOURCE_DIR}/palka_core" "../palka_core/*.cpp")
#file(GLOB GLAD_SRC RELATIVE "${CMAKE_SOURCE_DIR}/palka_core" "../glad/src/*.c")
#
#set(HEADER_SOURCE ReflectionDebug.h Drawable.h)
#if (WIN32)
#    set(RTTR_DIR "E:\\Link\\rttr_gcc\\cmake")
#    set(FREETYPE_DIR D:\\freetype-master\\ft_gcc\\lib\\cmake\\freetype)
#    set(CMAKE_PREFIX_PATH D:\\freetype-master\\ft_gcc E:\\Link\\glfw_gcc12 D:\\freeglut-3.2.1\\build\\lib\\cmake\\FreeGLUT)
#    set(GLM_ROOT_DIR "E:\\Link\\glm")
#    find_package(glfw3 REQUIRED)
#    find_package(GLM REQUIRED)
#    find_package(OpenGL REQUIRED)
#    find_package(RTTR CONFIG REQUIRED Core)
#    add_library(PalkaCore SHARED ${CORE_SRC} ${HEADER_SOURCE})
#
#    target_include_directories(PalkaCore PUBLIC ${OPENGL_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})
#
#    target_link_libraries(PalkaCore PUBLIC ${OPENGL_LIBRARIES} ${GLM_LIBRARY_DIRS} STB IMGUI_GLFW IMGUI GLAD glfw RTTR::Core imm32 ws2_32 dwmapi mingw32)
#    target_compile_definitions(PalkaCore PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD GL_GLEXT_PROTOTYPES=1)
#endif (WIN32)
#IF (UNIX)
#    set(CMAKE_PREFIX_PATH /lib/cmake/glm /lib/share/rttr/cmake/)
#
#    find_package(glfw3 REQUIRED)
#    find_package(glm REQUIRED)
#
#    find_package(OpenGL REQUIRED)
#    find_package(RTTR CONFIG REQUIRED Core)
#    add_library(PalkaCore STATIC ${CORE_SRC} ${HEADER_SOURCE})
#    #include_directories(${GLM_INCLUDE_DIRS})
#    #link_directories(${GLM_LIBRARY_DIRS})
#    target_include_directories(PalkaCore PUBLIC ${OPENGL_INCLUDE_DIRS})
#    # imm32 ws2_32 dwmapi mingw32
#
#    target_link_libraries(PalkaCore PUBLIC ${OPENGL_LIBRARIES} glm::glm STB IMGUI_GLFW IMGUI GLAD glfw RTTR::Core)
#    target_compile_definitions(PalkaCore PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD GL_GLEXT_PROTOTYPES=1)
#endif (UNIX)
#
