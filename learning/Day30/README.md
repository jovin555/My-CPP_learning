# Day 30: Capstone Project — UART Command Shell + GPIO + Sensor
**Platform:** nRF52840 | **RTOS:** Zephyr RTOS

## Learning Objectives
- Full embedded C++ application
- UART command shell with parser
- GPIO LED control from commands
- Internal temperature sensor
- Multi-threaded Zephyr application
- OOP driver architecture
- State machine for shell

## Project Description
A complete embedded application demonstrating all 30 days of learning:
- **UART command shell**: parse commands over serial (AT-style)
- **LED control**: `led on/off/blink <0-3>` commands
- **Sensor**: `temp` command reads nRF52840 internal temperature
- **Status**: `status` prints uptime, thread count, error count
- **Architecture**: HAL → Driver → App, RAII, Singleton, FSM, threads

## Commands
```
led on 0       -> Turn on LED 0
led off 0      -> Turn off LED 0
led blink 0    -> Blink LED 0 at 1Hz
temp           -> Read internal temperature
status         -> Show system status
help           -> List all commands
```

## Build
```bash
west build -b nrf52840dk/nrf52840 .
west flash
# Open serial terminal at 115200 baud
picocom -b 115200 /dev/ttyACM0
```


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
