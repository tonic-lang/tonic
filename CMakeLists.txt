cmake_minimum_required(VERSION 3.25)
project(tonic)

set(CMAKE_CXX_STANDARD 23)

include_directories(compiler/include compiler/include/frontend compiler/include/backend compiler/include/analyzers compiler/include/tonic)

# gtest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

if (WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif (WIN32)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_subdirectory(compiler)
