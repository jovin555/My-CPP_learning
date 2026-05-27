# Day 26: Ring Buffers & Lock-free ISR Communication
**Platform:** nRF52840 | **RTOS:** Zephyr RTOS

## Learning Objectives
- K_MSGQ_DEFINE for ISR-to-thread messaging
- k_fifo for dynamic messages
- Ring buffer (SPSC)
- Lock-free single-producer single-consumer
- Zephyr ring_buf API
- Memory barriers in embedded

## Key Concepts
- `K_MSGQ_DEFINE` is the safest way to pass data from ISR to thread in Zephyr
- Ring buffer SPSC (Single Producer Single Consumer) is inherently lock-free on single-core
- `k_fifo` uses dynamic allocation — avoid in ISR unless pre-allocated
- Zephyr's `ring_buf` API handles byte streams (e.g., UART receive)
- Memory barriers (`__DMB()`) ensure ordering on multi-core or with DMA


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
