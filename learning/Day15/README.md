# Day 15: Classes & Objects in Embedded
**Platform:** nRF52840 | **RTOS:** Zephyr RTOS

## Learning Objectives
- Class basics in C++
- Access specifiers (public/private/protected)
- Member functions
- Embedded OOP rules
- No RTTI, no exceptions
- Object size awareness

## Key Concepts
- Classes are zero-cost when designed correctly (no virtual, no RTTI)
- Always use `private` data members, expose only what's needed
- Prefer composition over inheritance for small embedded classes
- Use `static_assert(sizeof(MyClass) == N)` to catch unexpected padding
- Compile with `-fno-rtti -fno-exceptions` in embedded (Zephyr does this by default)


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
