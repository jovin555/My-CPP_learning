# Day 6: Functions, Inline & Function Pointers
**Platform:** nRF52840 | **RTOS:** Zephyr RTOS

## Learning Objectives
- Function declaration vs definition
- inline: hint to compiler to expand at call site
- Function pointers: void (*fp)(void *)
- Callback pattern for event-driven embedded code
- Zephyr GPIO callback (gpio_callback structure)
- static functions for file-scope linkage

## Key Concepts
- `inline` eliminates call overhead for small, hot functions
- Function pointers are how C-style callbacks work (Zephyr uses them extensively)
- Always pass a `void *user_data` context pointer with callbacks
- Zephyr's `gpio_init_callback()` + `gpio_add_callback()` is the standard pattern

## Build & Run
```bash
cd /home/eva/workspace/My-CPP_learning/src/day06
west build -b nrf52840dk/nrf52840 .
west flash
# View logs via RTT or UART
```

## Practice Tasks
1. Build the example and observe the output.
2. Modify one variable or configuration option and rebuild.
3. Add a `LOG_INF()` call showing a new value.
4. Check the generated map file: `build/zephyr/zephyr.map`
