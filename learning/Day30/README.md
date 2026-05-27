# Day 30: Capstone Project — Embedded C++ Application
**Platform:** nRF52840 | **RTOS:** Zephyr RTOS

---

## Learning Objectives

### Project Overview
This capstone demonstrates everything from the 30-day plan in a single, complete Zephyr application running on nRF52840:
- **UART command shell** with AT-style command parsing
- **LED control** via commands with a background blink FSM
- **Temperature monitoring** using the nRF52840 internal temperature sensor
- **Multi-threaded design**: shell thread + blink thread + main heartbeat
- **OOP architecture**: Singleton `LedManager`, RAII locks, layered design

### Architecture
```
┌─────────────────────────────────────────────────┐
│                 Application Layer               │
│  UART Shell Thread │ Blink Thread │ Main Thread │
├─────────────────────────────────────────────────┤
│                  Driver Layer                    │
│   LedManager (Singleton) │ UartRx (Ring buf)    │
│   NrfInternalTemp (Sensor API wrapper)           │
├─────────────────────────────────────────────────┤
│               Zephyr Kernel Layer               │
│  k_thread │ k_sem │ k_mutex │ k_msgq │ LOG      │
├─────────────────────────────────────────────────┤
│              Hardware (nRF52840)                │
│  UARTE0 │ GPIO (LEDs) │ TEMP peripheral          │
└─────────────────────────────────────────────────┘
```

### Commands
| Command | Description |
|---|---|
| `led on <0-3>` | Turn LED N on |
| `led off <0-3>` | Turn LED N off |
| `led blink <0-3>` | Blink LED N at 1Hz |
| `temp` | Read internal die temperature |
| `status` | Show uptime and error count |
| `help` | List all commands |

### C++ Concepts Used
- **Day 3:** `uint8_t`, `constexpr`, `volatile`
- **Day 4:** `BIT()`, bit masking for LED pattern
- **Day 6:** Function pointers for command dispatch table
- **Day 7:** `LOG_MODULE_REGISTER`, `LOG_INF/ERR`
- **Day 13:** ISR + `k_sem_give` for UART RX
- **Day 15:** `LedManager` class, `PidController` class
- **Day 16:** RAII `ScopedLock` for UART TX mutex
- **Day 20:** Singleton `LedManager::instance()`
- **Day 21:** `ITemperatureSensor` HAL + `NrfInternalTemp` driver
- **Day 22:** Return codes with `-EINVAL`, `__ASSERT`
- **Day 25:** LED blink state machine with `enum class`
- **Day 27:** `K_THREAD_DEFINE` for shell and blink threads
- **Day 28:** `K_SEM_DEFINE`, `K_MUTEX_DEFINE` for synchronization

### Build & Flash
```bash
cd /home/eva/workspace/My-CPP_learning/src/day30
west build -b nrf52840dk/nrf52840 .
west flash
# Open serial terminal at 115200 baud:
picocom -b 115200 /dev/ttyACM0
# Then type: help
```

### Extension Ideas
After completing the capstone, extend it:
1. Add BLE GATT characteristic that mirrors the UART commands
2. Add ADC channel reading with `adc` command
3. Add SPI sensor (BME280) with `sensor` command
4. Add Flash storage for command history and configuration
5. Add OTA firmware update support via Zephyr MCUboot

---

## Practice Tasks

1. Build and flash the capstone — test all 6 commands over serial
2. Add an `adc <channel>` command that reads an ADC channel and reports voltage in mV
3. Add a command history buffer (last 8 commands) with UP arrow recall (ANSI escape codes)
4. Port the UART shell to BLE: the same command handler, but input via a BLE GATT characteristic

---

## Build & Run

```bash
cd /home/eva/workspace/My-CPP_learning/src/day30
west build -b nrf52840dk/nrf52840 .
west flash
# View output via RTT or UART serial (115200 baud)
west attach   # RTT viewer (requires J-Link)
```

## Resources
- [Zephyr Docs](https://docs.zephyrproject.org)
- [nRF52840 Product Specification](https://infocenter.nordicsemi.com/topic/struct_nrf52/struct/nrf52840.html)
- [Nordic DevZone](https://devzone.nordicsemi.com)
- Source: [src/day30/main.cpp](../../src/day30/main.cpp)
