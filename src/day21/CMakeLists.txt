cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(day21_layered_driver_architecture)
target_sources(app PRIVATE main.cpp)
