# Day 12: Arrays, Buffers & Static Allocation
**Platform:** nRF52840 | **RTOS:** Zephyr RTOS

## Learning Objectives
- Fixed-size arrays vs dynamic allocation
- std::array in embedded
- Static memory pools
- K_MEM_SLAB
- Ring buffer basics
- Avoiding heap

## Key Concepts
- Prefer `static` arrays over `new`/`malloc` in embedded
- `std::array` is zero-overhead and bounds-checkable
- Zephyr `k_mem_slab` provides deterministic allocation
- Never use `std::vector` or `std::string` in bare-metal embedded


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
