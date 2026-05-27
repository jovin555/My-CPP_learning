# Day 5: Control Flow & Lookup Tables
**Platform:** nRF52840 | **RTOS:** Zephyr RTOS

## Learning Objectives
- if/else if/else
- switch-case (with and without fall-through)
- Ternary operator
- Lookup tables: O(1) vs O(n) if/else chains
- constexpr arrays for ROM-stored tables
- Avoiding complex branches in ISRs

## Key Concepts
- Lookup tables trade memory for speed — essential for time-critical code
- `constexpr` arrays go to Flash (.rodata) — no RAM cost
- In ISRs: prefer lookup tables over long if/else chains (predictable execution time)
- `switch` on `enum class` with no default → compiler warns on missing cases

## Build & Run
```bash
cd /home/eva/workspace/My-CPP_learning/src/day05
west build -b nrf52840dk/nrf52840 .
west flash
# View logs via RTT or UART
```

## Practice Tasks
1. Build the example and observe the output.
2. Modify one variable or configuration option and rebuild.
3. Add a `LOG_INF()` call showing a new value.
4. Check the generated map file: `build/zephyr/zephyr.map`
