cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS ${ZEPHYR_BASE})
project(day09_memory)
target_sources(app PRIVATE main.cpp)
