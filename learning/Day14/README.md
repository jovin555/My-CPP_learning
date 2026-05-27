# Day 14: GPIO & UART with Zephyr
**Platform:** nRF52840 | **RTOS:** Zephyr RTOS

## Learning Objectives
- gpio_pin_configure_dt with DTS
- UART IRQ-driven RX
- Devicetree overlays for custom boards
- DTS aliases and chosen nodes
- uart_irq_callback_set
- nRF52840 UARTE peripheral

## Key Concepts
- Always use devicetree (`DT_ALIAS`, `DT_NODELABEL`) not hardcoded addresses
- `uart_irq_callback_set()` for interrupt-driven UART RX
- Overlay files (`.overlay`) let you customize DTS without modifying board files
- `gpio_dt_spec` and `uart_dt_spec` are the type-safe DTS handle structs


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
