# Day 2: Bare Metal vs Zephyr Hello World — LED Blink
**Platform:** nRF52840 | **RTOS:** Zephyr RTOS

## Learning Objectives
- Bare metal startup sequence (vector table, reset handler, main)
- Zephyr application lifecycle
- GPIO API: gpio_pin_configure_dt, gpio_pin_toggle_dt
- Devicetree aliases (led0, led1...)
- struct gpio_dt_spec
- k_msleep for cooperative yielding

## Key Concepts
- In Zephyr, LEDs are described in DTS as `aliases { led0 = &led_0; }`
- `GPIO_DT_SPEC_GET` retrieves the pin spec from DTS at compile time
- Always call `gpio_is_ready_dt()` before using a GPIO handle
- `k_msleep()` yields the CPU — other threads can run during the sleep

## Build & Run
```bash
cd /home/eva/workspace/My-CPP_learning/src/day02
west build -b nrf52840dk/nrf52840 .
west flash
# View logs via RTT or UART
```

## Practice Tasks
1. Build the example and observe the output.
2. Modify one variable or configuration option and rebuild.
3. Add a `LOG_INF()` call showing a new value.
4. Check the generated map file: `build/zephyr/zephyr.map`
