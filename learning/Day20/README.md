# Day 20: Singleton & Static Peripheral Managers
**Platform:** nRF52840 | **RTOS:** Zephyr RTOS

## Learning Objectives
- Singleton design pattern
- Monostate pattern
- K_MUTEX_DEFINE for C++ singletons
- Static peripheral manager
- Thread-safe singleton in Zephyr
- Global object initialization order

## Key Concepts
- Singleton ensures only one instance of a peripheral manager exists
- Use Meyers Singleton (local static) for thread-safe lazy initialization (C++11+)
- Zephyr static objects (`K_MUTEX_DEFINE`) are initialized before `main()`
- Prefer dependency injection over globals for testability
- Global constructor ordering (static init order fiasco) — use `init()` functions instead


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
