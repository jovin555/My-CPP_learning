# Day 25: State Machines
**Platform:** nRF52840 | **RTOS:** Zephyr RTOS

## Learning Objectives
- enum class FSM
- Template FSM pattern
- Hierarchical state machines
- Zephyr SMF (State Machine Framework)
- Event-driven vs polling FSM
- Guard conditions and actions

## Key Concepts
- State machines are the backbone of embedded firmware
- `enum class` prevents implicit state value mixing
- Zephyr SMF provides a hierarchical state machine framework
- Event-driven FSM: state changes driven by external events (button, timer, UART)
- Always define: states, events, transitions, entry/exit actions


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
