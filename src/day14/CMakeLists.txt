cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(day14_gpio_&_uart_with_zephyr)
target_sources(app PRIVATE main.cpp)
