cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(day16_constructors,_destructors_&_ra)
target_sources(app PRIVATE main.cpp)
