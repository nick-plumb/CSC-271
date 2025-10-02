cmake_minimum_required(VERSION 3.29)
# version number
project(CSC271)
# project name
set(CMAKE_CXX_STANDARD 20)
# C++ standard

# adds GLFW and GLAD directories to the search path
include_directories(${CMAKE_SOURCE_DIR}/glfw-3.4.bin.WIN64/include
${CMAKE_SOURCE_DIR}/glad/include
${CMAKE_SOURCE_DIR}/include
${CMAKE_SOURCE_DIR}/src)

# adds the directory containing the compiled GLFW library to the link search path
link_directories(${CMAKE_SOURCE_DIR}/glfw-3.4.bin.WIN64/lib-mingw-w64)

# adds the GLAD source file
add_library(glad ${CMAKE_SOURCE_DIR}/glad/src/glad.c)

# Define the executable target that is built from main.cpp
add_executable(CSC271 main.cpp
        include/shaderprogram.h
        src/shaderprogram.cpp
        src/stb_image.cpp
        include/mesh.h
        src/mesh.cpp)

add_compile_definitions(SHADER_DIR="${CMAKE_SOURCE_DIR}/shaders/")
add_compile_definitions(ASSET_DIR="${CMAKE_SOURCE_DIR}/assets/")

target_link_libraries(CSC271
        glad
        glfw3.dll
)
