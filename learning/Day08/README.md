# Day 8: ARM Cortex-M Architecture
**Platform:** nRF52840 | **RTOS:** Zephyr RTOS

## Learning Objectives
- ARM Cortex-M4 core in nRF52840 (no FPU variant: M4F with FPU)
- nRF52840 memory map: Flash 0x0, RAM 0x2000_0000, Peripherals 0x4000_0000
- NVIC: Nested Vectored Interrupt Controller
- SysTick: 24-bit countdown timer (Zephyr tick source)
- Vector table: interrupt handler addresses at Flash base
- Fault handlers: HardFault, BusFault, UsageFault
- DWT cycle counter for timing measurements

## Key Concepts
- The vector table lives at address 0x00000000 (start of Flash on nRF52840)
- NVIC priority: lower number = higher priority (0 = highest)
- SysTick is what drives `k_uptime_get_32()` and `k_msleep()` in Zephyr
- DWT_CYCCNT counts CPU cycles — useful for measuring execution time
- Hard Fault = CPU panic: bad memory access, bad instruction, stack overflow

## Build & Run
```bash
cd /home/eva/workspace/My-CPP_learning/src/day08
west build -b nrf52840dk/nrf52840 .
west flash
# View logs via RTT or UART
```

## Practice Tasks
1. Build the example and observe the output.
2. Modify one variable or configuration option and rebuild.
3. Add a `LOG_INF()` call showing a new value.
4. Check the generated map file: `build/zephyr/zephyr.map`
