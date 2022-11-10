cmake_minimum_required(VERSION 3.10)

project(RST VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_BUILD_TYPE "Release")

add_executable(rst "")
target_include_directories(rst PUBLIC ${CMAKE_SOURCE_DIR})
target_sources(rst
  PUBLIC
  dgen.cxx
)
