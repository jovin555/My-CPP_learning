cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(day15_classes_&_objects_in_embedded)
target_sources(app PRIVATE main.cpp)
