cmake_minimum_required(VERSION 3.28)

project(aoc-2024-22 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)

qt_add_executable(aoc-2024-22
  main.cpp
  input.txt
  test.txt
)

target_link_libraries(aoc-2024-22 PRIVATE Qt::Core)
