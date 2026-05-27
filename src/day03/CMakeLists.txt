cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS ${ZEPHYR_BASE})
project(day03_data_types)
target_sources(app PRIVATE main.cpp)
