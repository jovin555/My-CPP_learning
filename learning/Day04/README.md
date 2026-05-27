# Day 4: Bitwise Operations & Register Manipulation
**Platform:** nRF52840 | **RTOS:** Zephyr RTOS

## Learning Objectives
- AND (&), OR (|), XOR (^), NOT (~)
- Left shift (<<) and right shift (>>)
- Bit set: reg |= BIT(n)
- Bit clear: reg &= ~BIT(n)
- Bit toggle: reg ^= BIT(n)
- Bit test: (reg >> n) & 1
- GENMASK for multi-bit fields
- Zephyr BIT() and sys_util.h macros

## Key Concepts
- Bitwise operations are the most critical skill for register-level embedded programming
- `BIT(n)` from Zephyr's `<zephyr/sys/util.h>` is safer than `(1 << n)`
- `GENMASK(high, low)` creates a bitmask for a register field
- Always use parentheses around macro arguments to avoid operator precedence bugs

## Build & Run
```bash
cd /home/eva/workspace/My-CPP_learning/src/day04
west build -b nrf52840dk/nrf52840 .
west flash
# View logs via RTT or UART
```

## Practice Tasks
1. Build the example and observe the output.
2. Modify one variable or configuration option and rebuild.
3. Add a `LOG_INF()` call showing a new value.
4. Check the generated map file: `build/zephyr/zephyr.map`
