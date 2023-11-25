cmake_minimum_required(VERSION 3.22.1)

project(quick_sort_bench)

find_package(Parlay REQUIRED)

add_executable(quick_sort_benchmark
               main.cpp
               quick_sort_par.h
               quick_sort_seq.h
               quick_sort_par.cpp
               quick_sort_seq.cpp
               quick_sort_par_theory_optimal.cpp)

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

target_link_libraries(quick_sort_benchmark PUBLIC Parlay::parlay)

