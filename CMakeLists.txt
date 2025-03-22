cmake_minimum_required(VERSION 3.17.2)

# set(CMAKE_C_COMPILER "/usr/bin/gcc")
# set(CMAKE_CXX_COMPILER "/usr/bin/g++")
# set(CMAKE_C_COMPILER "/usr/bin/clang")
# set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(monogame.shadercompiler)

# Set library output directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(LIBRARY_SEARCH_PATHS
    "external/SPIRV-Cross/build"
    "external/DirectXShaderCompiler/build/lib"
)

# Define sources
set(SOURCES src/spirv_wrapper.c src/dxc_wrapper.cpp)

include_directories(include external/SPIRV-Cross external/DirectXShaderCompiler/include)

# Define the shared library
add_library(monogame.shadercompiler SHARED ${SOURCES})

# Use file(GLOB ...) to collect all .a files in the specified directories
file(GLOB SPIRV_CROSS_LIBS "external/SPIRV-Cross/build/*.a")
file(GLOB DXCOMPILER_LIBS "external/DirectXShaderCompiler/build/lib/*.dylib")

# Link against SPIRV-Cross
target_link_libraries(monogame.shadercompiler
    ${DXCOMPILER_LIBS}
    ${SPIRV_CROSS_LIBS}
)

# Set platform-specific properties
if(WIN32)
    set_target_properties(monogame.shadercompiler PROPERTIES SUFFIX ".dll")
elseif(APPLE)
    set_target_properties(monogame.shadercompiler PROPERTIES SUFFIX ".dylib")
else()
    set_target_properties(monogame.shadercompiler PROPERTIES SUFFIX ".so")
endif()
