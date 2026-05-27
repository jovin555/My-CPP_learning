# Day 27: Zephyr Threads & Scheduling
**Platform:** nRF52840 | **RTOS:** Zephyr RTOS

## Learning Objectives
- K_THREAD_DEFINE
- k_thread_create
- Thread priorities (cooperative vs preemptive)
- k_sleep / k_msleep
- k_yield
- Real-time constraints & WCET
- Thread stack analysis

## Key Concepts
- Zephyr uses a priority-based preemptive scheduler
- Priorities 0-14: preemptive threads (lower number = higher priority)
- Priority -1 to -CONFIG_NUM_COOP_PRIORITIES: cooperative threads (no preemption)
- Always check stack usage with `CONFIG_THREAD_STACK_INFO`
- Worst-Case Execution Time (WCET) analysis is critical for real-time systems


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
