# Day 7: Debugging from Day 1
**Platform:** nRF52840 | **RTOS:** Zephyr RTOS

## Learning Objectives
- Zephyr LOG subsystem (LOG_INF, LOG_ERR, LOG_DBG, LOG_WRN)
- LOG_MODULE_REGISTER for per-file log control
- CONFIG_LOG_DEFAULT_LEVEL
- __ASSERT() for debug-time assertions
- GDB over J-Link (west debug)
- RTT (Real-Time Transfer) logging
- k_oops() and k_panic()

## Key Concepts
- Start logging from Day 1 — instrumented code is debuggable code
- LOG_DBG messages are stripped in release builds (CONFIG_LOG_DEFAULT_LEVEL < 4)
- `__ASSERT(cond, msg)` is stripped when CONFIG_ASSERT=n (production builds)
- RTT is faster than UART for logging — no interrupt overhead
- `west debug` starts a GDB session via J-Link

## Build & Run
```bash
cd /home/eva/workspace/My-CPP_learning/src/day07
west build -b nrf52840dk/nrf52840 .
west flash
# View logs via RTT or UART
```

## Practice Tasks
1. Build the example and observe the output.
2. Modify one variable or configuration option and rebuild.
3. Add a `LOG_INF()` call showing a new value.
4. Check the generated map file: `build/zephyr/zephyr.map`
