# Day 16: Constructors, Destructors & RAII
**Platform:** nRF52840 | **RTOS:** Zephyr RTOS

## Learning Objectives
- Default/parameterized constructors
- Destructors and cleanup
- Copy vs Move constructors
- RAII pattern in embedded
- ScopedLock for Zephyr mutex
- Deferred initialization pattern

## Key Concepts
- RAII (Resource Acquisition Is Initialization) is the most important embedded C++ pattern
- Destructors guarantee cleanup even on early returns
- Use `= delete` to prevent unwanted copies of hardware handles
- Zephyr mutexes, GPIOs, and DMA handles are perfect RAII candidates
- Deferred init: construct the object first, call `init()` separately (avoids global constructor ordering issues)


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
