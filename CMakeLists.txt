cmake_minimum_required(VERSION 3.10)
project(Beta-reduction)

option(COMPILE_TESTS "Build tests" ON)

# By default select Debug build type
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "${CMAKE_BINARY_DIR}/target")

if (MSVC)
    add_compile_options(/MT /MTd /EHsc)
    add_compile_options("$<$<CONFIG:Release>:/O2>")

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG  "${CMAKE_BINARY_DIR}/target")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE  "${CMAKE_BINARY_DIR}/target")
else()
    # -DELPP_THREAD_SAFE -- for easylogging++ thread safety
    set(BUILD_OPTS "-DELPP_THREAD_SAFE")

    # RELEASE build options
    set(RELEASE_BUILD_OPTS "-O3")

    # Set common DEBUG build options
    string(JOIN ";" DEBUG_BUILD_OPTS
        "-g"
        "-Wall"
        "-Wextra"
        "-Werror"
        "-fno-omit-frame-pointer"
        "-fno-optimize-sibling-calls"
        "-O0")

    add_compile_options("$<$<CONFIG:Debug>:${DEBUG_BUILD_OPTS}>")

    add_compile_options("$<$<CONFIG:Release>:${RELEASE_BUILD_OPTS}>")
endif()

# configuring boost
set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost 1.72.0 REQUIRED COMPONENTS program_options)
include_directories(${Boost_INCLUDE_DIRS})
message(${Boost_INCLUDE_DIRS})

# providing subdirectories for header inclusion
include_directories(
        "src"
        "tests"
)

# GTest
if (COMPILE_TESTS)
    add_subdirectory("lib/googletest")
endif()

# easylogging++
include_directories(SYSTEM "lib/easyloggingpp/src")

set( CMAKE_BUILD_TYPE_COPY "${CMAKE_BUILD_TYPE}" )
set( CMAKE_BUILD_TYPE "Release" )
option(build_static_lib "Build easyloggingpp as a static library" ON)
add_subdirectory("lib/easyloggingpp")
set( CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE_COPY} )

add_subdirectory("src")

add_subdirectory("cli")

# adding submodules
if (COMPILE_TESTS)
    add_subdirectory("tests")
endif()
