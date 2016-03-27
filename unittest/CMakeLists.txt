cmake_minimum_required(VERSION 2.8.7)

add_definitions("-Wall -std=c++0x")
include_directories(
  ../include
)

find_package (Threads)

add_executable(testrunner
  main.cpp
  test_request.cpp
)
target_link_libraries(testrunner rapid ${CMAKE_THREAD_LIBS_INIT})
