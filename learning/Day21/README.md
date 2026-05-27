# Day 21: Layered Driver Architecture
**Platform:** nRF52840 | **RTOS:** Zephyr RTOS

## Learning Objectives
- HAL layer design
- Driver layer (device-specific)
- Application layer
- SPI & I2C Zephyr wrapper classes
- Separation of concerns
- Driver registration pattern

## Key Concepts
- 3-layer architecture: HAL → Driver → Application
- HAL: hardware-agnostic interface (abstract C++ class)
- Driver: implements HAL for specific hardware (e.g., BME280 sensor)
- Application: uses Driver without knowing hardware details
- Zephyr's sensor subsystem (`<zephyr/drivers/sensor.h>`) follows this exact pattern


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
