# Day 11: Structs, Bit-fields & Unions
**Platform:** nRF52840 | **RTOS:** Zephyr RTOS

## Learning Objectives
- Structs for grouping related data
- Bit-fields for register mapping
- __packed attribute
- Unions for type punning
- Zephyr sys_ register structs

## Key Concepts
- Use `__packed` to prevent padding in register-mapped structs
- Bit-fields map directly to hardware register bits
- Unions allow re-interpreting the same memory as different types
- Always use fixed-width types inside register structs


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
