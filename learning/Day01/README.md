# Day 1: Intro to Embedded & Zephyr Toolchain
**Platform:** nRF52840 | **RTOS:** Zephyr RTOS

## Learning Objectives
- What is an embedded system
- nRF52840 SoC overview (ARM Cortex-M4, 64MHz, 1MB Flash, 256KB RAM)
- Zephyr RTOS architecture
- West meta-tool (build/flash/debug)
- CMakeLists.txt & Kconfig
- Devicetree (DTS) overview

## Key Concepts
- West is Zephyr's Swiss-army tool: `west build`, `west flash`, `west debug`
- Kconfig (`prj.conf`) configures what Zephyr subsystems are compiled in
- Devicetree describes hardware topology — not code, not config, but *structure*
- nRF52840DK board target: `nrf52840dk/nrf52840`

## Build & Run
```bash
cd /home/eva/workspace/My-CPP_learning/src/day01
west build -b nrf52840dk/nrf52840 .
west flash
# View logs via RTT or UART
```

## Practice Tasks
1. Build the example and observe the output.
2. Modify one variable or configuration option and rebuild.
3. Add a `LOG_INF()` call showing a new value.
4. Check the generated map file: `build/zephyr/zephyr.map`
