cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS ${ZEPHYR_BASE})
project(day01_hello)
target_sources(app PRIVATE main.cpp)
