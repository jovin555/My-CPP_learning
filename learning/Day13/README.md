# Day 13: Interrupts & ISR Safety
**Platform:** nRF52840 | **RTOS:** Zephyr RTOS

## Learning Objectives
- Zephyr IRQ API
- ISR_DIRECT_DECLARE
- volatile in ISR context
- irq_lock / irq_unlock
- k_sem_give from ISR
- GPIOTE on nRF52840

## Key Concepts
- ISRs must be fast — no blocking, no heap allocation, no printk
- Use `volatile` for variables shared between ISR and main context
- Use Zephyr `k_sem` or `k_msgq` to communicate from ISR to thread
- `irq_lock()` disables all interrupts (use sparingly, very short sections)


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
