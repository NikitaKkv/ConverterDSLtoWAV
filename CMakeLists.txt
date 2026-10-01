cmake_minimum_required(VERSION 3.24)

project(
    itmoloops
    VERSION 1.0
    DESCRIPTION "C++ Year 2025 labwork5"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)


add_subdirectory(lib)
add_subdirectory(bin)
