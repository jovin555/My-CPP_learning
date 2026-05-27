# Day 10: Pointers & References for Hardware Access
**Platform:** nRF52840 | **RTOS:** Zephyr RTOS

## Learning Objectives
- Pointer basics: declaration, dereferencing, NULL
- volatile pointer: volatile uint32_t *reg_ptr (for MMIO)
- const pointer vs pointer to const
- References: alias with no null, no reassign, no arithmetic
- reinterpret_cast for MMIO register access (C++ style)
- extern "C": C linkage for vendor library compatibility
- nullptr vs NULL (prefer nullptr in C++)

## Key Concepts
- `volatile uint32_t *` tells the compiler every access to this address is a real memory operation
- Without `volatile`, the compiler may cache MMIO register reads in a CPU register — breaking hardware access
- `reinterpret_cast<volatile uint32_t *>(0x40000504UL)` is the C++ way to access a peripheral register
- `extern "C"` prevents C++ name mangling — required when calling vendor C SDK functions
- Prefer references over pointers for parameters when null is not a valid value

## Build & Run
```bash
cd /home/eva/workspace/My-CPP_learning/src/day10
west build -b nrf52840dk/nrf52840 .
west flash
# View logs via RTT or UART
```

## Practice Tasks
1. Build the example and observe the output.
2. Modify one variable or configuration option and rebuild.
3. Add a `LOG_INF()` call showing a new value.
4. Check the generated map file: `build/zephyr/zephyr.map`
