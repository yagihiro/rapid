cmake_minimum_required(VERSION 2.8.7)

add_definitions("-Wall -std=c++0x")
include_directories(
  ../include
)
add_library(rapid STATIC
  rapid.cpp
  request.cpp
  response.cpp
  route.cpp
  routing_dispatcher.cpp
)
