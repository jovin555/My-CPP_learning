# Day 17: Inheritance & Hardware Abstraction Layer
**Platform:** nRF52840 | **RTOS:** Zephyr RTOS

## Learning Objectives
- Base classes and derived classes
- Abstract base class with pure virtual
- Virtual function vtable overhead
- Zephyr driver model analogy
- HAL design pattern
- Interface classes in embedded

## Key Concepts
- Use abstract base classes to define hardware interfaces (Sensor, Actuator, Comm)
- Virtual functions cost one pointer per object (vtable ptr) — acceptable for drivers
- Prefer non-virtual functions unless runtime polymorphism is truly needed
- Zephyr's own driver model uses a similar struct-of-function-pointers approach
- CRTP (Curiously Recurring Template Pattern) gives polymorphism with zero cost (Day 18)


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
