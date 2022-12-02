cmake_minimum_required(VERSION 3.22)
project(aoc-2022-02a LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(aoc-2022-02a main.cpp)

target_compile_options(aoc-2022-02a PRIVATE
  -Wall -Wextra -Wpedantic -Wconversion
)