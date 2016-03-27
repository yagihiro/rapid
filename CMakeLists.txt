cmake_minimum_required(VERSION 2.8.7)

add_definitions("-Wall -std=c++0x")
include_directories(
  include
)

find_package (Threads)

add_subdirectory(src)
add_subdirectory(unittest)
