# Day 19: Operator Overloading & Type-safe Registers
**Platform:** nRF52840 | **RTOS:** Zephyr RTOS

## Learning Objectives
- Operator overloading basics
- Type-safe GPIO pin wrapper
- Register class with |= &= operators
- Strong typedef pattern
- Preventing implicit conversions
- Units library concept

## Key Concepts
- Operator overloading lets register access look like: `reg |= GPIO_PIN_5`
- Strong typedefs prevent mixing up `PinNumber` with raw `uint8_t`
- Never overload `&&`, `||`, `,` — they lose short-circuit semantics
- The goal: make incorrect hardware access **not compile**


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
