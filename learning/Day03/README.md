# Day 3: Embedded Data Types & Keywords
**Platform:** nRF52840 | **RTOS:** Zephyr RTOS

## Learning Objectives
- Fixed-width types: uint8_t, uint16_t, uint32_t (from <stdint.h>)
- volatile: prevents compiler optimization for hardware-shared variables
- const: read-only data stored in Flash (.rodata)
- constexpr (C++11): compile-time constants
- static: file-scope visibility control
- sizeof guarantees for embedded portability

## Key Concepts
- Never use `int` or `long` in embedded without knowing their size — use `uint32_t` etc.
- `volatile` tells the compiler to always re-read a variable from memory
- `const` global variables go to Flash (saves precious RAM)
- `constexpr` is evaluated at compile time — zero runtime overhead

## Build & Run
```bash
cd /home/eva/workspace/My-CPP_learning/src/day03
west build -b nrf52840dk/nrf52840 .
west flash
# View logs via RTT or UART
```

## Practice Tasks
1. Build the example and observe the output.
2. Modify one variable or configuration option and rebuild.
3. Add a `LOG_INF()` call showing a new value.
4. Check the generated map file: `build/zephyr/zephyr.map`
