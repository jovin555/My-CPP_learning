cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(day22_error_handling_in_embedded)
target_sources(app PRIVATE main.cpp)
