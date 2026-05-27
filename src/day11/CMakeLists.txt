cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(day11_structs,_bit-fields_&_unions)
target_sources(app PRIVATE main.cpp)
