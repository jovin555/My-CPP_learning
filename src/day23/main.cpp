#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <array>
#include <tuple>

LOG_MODULE_REGISTER(day23_modern_cpp, LOG_LEVEL_INF);

// constexpr: computed at compile time, stored in Flash
constexpr uint32_t CPU_FREQ_HZ     = 64'000'000UL;  // C++14 digit separator
constexpr uint32_t TICK_PERIOD_NS  = 1'000'000'000UL / CPU_FREQ_HZ;

constexpr uint16_t adc_to_mv(uint16_t raw, uint16_t vref_mv = 3300)
{
    return static_cast<uint16_t>((uint32_t)raw * vref_mv / 4095U);
}

// static_assert on constexpr result
static_assert(adc_to_mv(2048) == 1650U, "Mid-scale should be half of 3300 mV");

// enum class: scoped, no implicit int conversion
enum class BaudRate : uint32_t {
    Baud9600   =   9'600UL,
    Baud115200 = 115'200UL,
    Baud1M     = 1'000'000UL,
};

// [[nodiscard]]: compiler warns if return value is ignored
[[nodiscard]] int configure_uart_baud(BaudRate baud)
{
    LOG_INF("UART baud: %u", static_cast<uint32_t>(baud));
    return 0;
}

// if constexpr (C++17): compile-time branching, no runtime overhead
template <typename T>
constexpr const char *type_name()
{
    if constexpr (sizeof(T) == 1) return "uint8_t";
    else if constexpr (sizeof(T) == 2) return "uint16_t";
    else if constexpr (sizeof(T) == 4) return "uint32_t";
    else return "unknown";
}

// Structured bindings (C++17): unpack struct/tuple/array
struct PinConfig { uint8_t pin; bool active_low; uint32_t debounce_ms; };

static PinConfig get_button_config()
{
    return {11, true, 50};
}

int main(void)
{
    LOG_INF("CPU: %u Hz, tick: %u ns", CPU_FREQ_HZ, TICK_PERIOD_NS);

    // constexpr ADC conversion
    for (uint16_t raw : {0, 1024, 2048, 4095}) {
        LOG_INF("ADC %u -> %u mV", raw, adc_to_mv(raw));
    }

    // enum class with switch
    BaudRate baud = BaudRate::Baud115200;
    switch (baud) {
    case BaudRate::Baud9600:   LOG_INF("9600 baud");   break;
    case BaudRate::Baud115200: LOG_INF("115200 baud"); break;
    case BaudRate::Baud1M:     LOG_INF("1M baud");     break;
    }

    // [[nodiscard]] — must use return value
    int ret = configure_uart_baud(BaudRate::Baud115200);
    if (ret != 0) { LOG_ERR("UART config failed"); }

    // if constexpr: resolved at compile time
    LOG_INF("uint8_t  -> %s", type_name<uint8_t>());
    LOG_INF("uint32_t -> %s", type_name<uint32_t>());

    // Structured bindings (C++17)
    auto [pin, active_low, debounce] = get_button_config();
    LOG_INF("Button: pin=%u active_low=%s debounce=%ums",
            pin, active_low ? "yes" : "no", debounce);

    while (1) { k_msleep(5000); }
    return 0;
}
