# Day 24: Memory Management Advanced
**Platform:** nRF52840 | **RTOS:** Zephyr RTOS

## Learning Objectives
- Placement new
- Memory pools (K_HEAP_DEFINE)
- Custom allocators concept
- Smart pointer trade-offs in embedded
- unique_ptr for RAII
- Memory pool patterns

## Key Concepts
- Avoid `new`/`delete` in embedded — use static pools or stack allocation
- `placement new` constructs an object in pre-allocated memory (no heap)
- Zephyr `K_HEAP_DEFINE` gives a bounded heap for controlled dynamic allocation
- `std::unique_ptr` is zero-overhead and safe (just a wrapper around a pointer)
- `std::shared_ptr` has atomic reference counting overhead — avoid in bare-metal ISRs


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
