cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(day26_ring_buffers_&_lock-free_isr_c)
target_sources(app PRIVATE main.cpp)
