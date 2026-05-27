cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(day12_arrays,_buffers_&_static_alloc)
target_sources(app PRIVATE main.cpp)
