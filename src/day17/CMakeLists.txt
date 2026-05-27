cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(day17_inheritance_&_hardware_abstrac)
target_sources(app PRIVATE main.cpp)
