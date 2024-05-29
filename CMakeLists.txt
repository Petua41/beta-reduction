cmake_minimum_required(VERSION 3.10)
project(Beta-reduction)

option(COMPILE_TESTS "Build tests" ON)

# By default select Debug build type
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "${CMAKE_BINARY_DIR}/target")

add_compile_options("$<$<CONFIG:Debug>:-g;-Wall;-Wextra;-Werror;-O0>")

add_compile_options("$<$<CONFIG:Release>:-O3>")

# configuring boost
set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost 1.72.0 REQUIRED COMPONENTS program_options)

set( CMAKE_BUILD_TYPE_COPY "${CMAKE_BUILD_TYPE}" )
set( CMAKE_BUILD_TYPE "Release" )
option(build_static_lib "Build easyloggingpp as a static library" ON)
add_subdirectory("vendor/easyloggingpp")
set( CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE_COPY} )

add_subdirectory("src")

add_subdirectory("cli")

# adding submodules
if (COMPILE_TESTS)
    add_subdirectory("tests")
    enable_testing()
endif()
