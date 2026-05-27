# Day 9: Memory Segments & Linker Scripts
**Platform:** nRF52840 | **RTOS:** Zephyr RTOS

## Learning Objectives
- .text: compiled code (Flash)
- .rodata: read-only data like const arrays (Flash)
- .data: initialized global variables (copied Flash→RAM at boot)
- .bss: zero-initialized globals (zeroed by startup code, RAM only)
- Stack: grows downward, local variables live here
- __noinit: survives resets without being zeroed (boot counters)
- Zephyr linker script: zephyr/kernel/arch/arm/core/cortex_m/link.ld
- west build produces zephyr.map — inspect it!

## Key Concepts
- Understanding memory segments is crucial for embedded — RAM is precious (256KB on nRF52840)
- `.noinit` section preserves variables across soft resets — useful for boot counters, crash logs
- The linker map file (`build/zephyr/zephyr.map`) shows where every symbol is placed
- `__attribute__((section(".noinit")))` places a variable in the noinit region
- `CONFIG_SRAM_BASE_ADDRESS` and `CONFIG_FLASH_BASE_ADDRESS` in Zephyr give the base addresses

## Build & Run
```bash
cd /home/eva/workspace/My-CPP_learning/src/day09
west build -b nrf52840dk/nrf52840 .
west flash
# View logs via RTT or UART
```

## Practice Tasks
1. Build the example and observe the output.
2. Modify one variable or configuration option and rebuild.
3. Add a `LOG_INF()` call showing a new value.
4. Check the generated map file: `build/zephyr/zephyr.map`
