# Day 23: Modern C++ in Embedded (C++11/14/17)
**Platform:** nRF52840 | **RTOS:** Zephyr RTOS

## Learning Objectives
- constexpr functions and variables
- nullptr vs NULL
- noexcept specifier
- scoped enum class
- Uniform initialization {}
- auto keyword
- if constexpr (C++17)
- Structured bindings (C++17)

## Key Concepts
- `constexpr` moves computation to compile time — great for lookup tables and config
- `enum class` prevents implicit conversion to int — type-safe state machines
- `auto` reduces verbosity but don't overuse — readability matters in embedded
- `if constexpr` replaces `#ifdef` for template branching
- `[[nodiscard]]` forces callers to check return values (error codes!)
- Zephyr C++ requires `CONFIG_CPP=y` and compiles with `-std=c++17` by default


## Build & Run
```bash
west build -b nrf52840dk/nrf52840 .
west flash
west espressif monitor   # or: west attach (J-Link RTT)
```

## Practice Tasks
1. Build and flash the example to nRF52840DK (or run in QEMU).
2. Modify the example to extend the concept.
3. Add a LOG_INF() call to trace a new variable.
4. Check the map file: `build/zephyr/zephyr.map`
