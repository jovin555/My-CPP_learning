cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(day13_interrupts_&_isr_safety)
target_sources(app PRIVATE main.cpp)
