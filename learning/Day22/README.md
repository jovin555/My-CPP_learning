# Day 22: Error Handling in Embedded
**Platform:** nRF52840 | **RTOS:** Zephyr RTOS

## Learning Objectives
- Error codes vs exceptions
- static_assert for compile-time checks
- __ASSERT for debug-time checks
- Return value patterns
- Result<T, E> pattern
- Zephyr error codes (-EINVAL etc)

## Key Concepts
- Never use C++ exceptions in embedded (`-fno-exceptions`)
- Return `int` error codes (Zephyr convention: 0 = OK, negative = error)
- `static_assert` catches errors at compile time — zero runtime cost
- `__ASSERT()` checks in debug builds, stripped in release
- Result type (like Rust's Result) can be done with a simple template


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
