# Day 29: Power Management & Compiler Optimization
**Platform:** nRF52840 | **RTOS:** Zephyr RTOS

## Learning Objectives
- Zephyr PM subsystem
- pm_device API
- Sleep states (idle, suspend, standby)
- CONFIG_PM_DEVICE
- Compiler flags: -Os, LTO, gc-sections
- Measuring power with nRF PPK2
- Low-power UART patterns

## Key Concepts
- nRF52840 supports multiple power modes: Active, Low-Power, System OFF
- Zephyr PM automatically enters idle states between tasks
- `pm_device_action_run()` controls individual peripheral power states
- Link-Time Optimization (`-flto`) removes unused code across compilation units
- `-ffunction-sections -fdata-sections --gc-sections` removes unused symbols
- Profile power with Nordic PPK2 (Power Profiler Kit 2) over J-Link


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
