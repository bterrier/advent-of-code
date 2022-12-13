cmake_minimum_required(VERSION 3.22)
project(aoc-2022-11 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(aoc-2022-11 main.cpp)

target_compile_options(aoc-2022-11 PRIVATE
  -Wall -Wextra -Wpedantic -Wconversion
)
