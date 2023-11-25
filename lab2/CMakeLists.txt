cmake_minimum_required(VERSION 3.22.1)

project(bfs_benchmark)

find_package(Parlay REQUIRED)

add_executable(bfs_benchmark
               main.cpp
               bfs_par.h
               bfs_seq.h
               vertex.h
               cubic_vertex.h
               cubic_vertex.cpp)

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

target_link_libraries(bfs_benchmark PUBLIC Parlay::parlay)

