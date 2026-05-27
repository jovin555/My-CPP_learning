# Day 28: Zephyr Synchronization Primitives
**Platform:** nRF52840 | **RTOS:** Zephyr RTOS

## Learning Objectives
- k_mutex (recursive-safe)
- k_sem (counting & binary)
- k_condvar (condition variables)
- k_poll for multi-event waiting
- Priority inversion & PI mutex
- RAII wrappers for Zephyr sync

## Key Concepts
- Mutex: mutual exclusion — only one thread at a time
- Semaphore: signaling — ISR gives, thread takes
- `k_poll` lets one thread wait on multiple events (semaphore, FIFO, etc.)
- Priority inversion: low-priority thread holds lock needed by high-priority thread
- Zephyr's PI (Priority Inheritance) mutex prevents priority inversion: `CONFIG_MUTEX_TIMEOUT_INHERITED`


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
