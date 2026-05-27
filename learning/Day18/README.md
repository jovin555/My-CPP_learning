# Day 18: Templates & Zero-Cost Abstractions
**Platform:** nRF52840 | **RTOS:** Zephyr RTOS

## Learning Objectives
- Function templates
- Class templates
- CRTP pattern
- template <typename Board>
- static_assert with templates
- Zero-overhead policy-based design

## Key Concepts
- Templates are resolved at compile time — no runtime overhead
- CRTP (Curiously Recurring Template Pattern) achieves polymorphism without vtable
- `static_assert` inside templates catches errors at compile time
- Use templates for generic drivers (e.g., `SpiDevice<ChipSelectPin>`)
- `constexpr if` (C++17) replaces template specialization for simple cases


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
