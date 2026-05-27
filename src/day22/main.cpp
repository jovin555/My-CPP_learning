#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <errno.h>

LOG_MODULE_REGISTER(day22_errors, LOG_LEVEL_INF);

// Simple Result type (no exceptions needed)
template <typename T>
struct Result {
    T    value;
    int  err;   // 0 = OK, negative = Zephyr error code

    static Result ok(T v)       { return {v, 0};    }
    static Result error(int e)  { return {{}, e};   }
    bool is_ok() const          { return err == 0;  }
};

// Compile-time checks — these fire at compile time, zero runtime cost
static_assert(sizeof(uint32_t) == 4, "uint32_t must be 4 bytes");
static_assert(sizeof(void *)   == 4, "Expected 32-bit pointers on Cortex-M");

// Function returning Zephyr-style error code
static int configure_sensor(uint8_t sensor_id, uint16_t sample_rate_hz)
{
    if (sensor_id > 7U)           return -EINVAL;   // Invalid argument
    if (sample_rate_hz == 0U)     return -EINVAL;
    if (sample_rate_hz > 1000U)   return -ENOTSUP;  // Not supported

    // Simulate configuration
    LOG_INF("Sensor %u configured at %u Hz", sensor_id, sample_rate_hz);
    return 0;  // Success
}

// Function returning Result<T>
static Result<uint16_t> read_adc(uint8_t channel)
{
    if (channel > 7U) return Result<uint16_t>::error(-EINVAL);

    // Simulate ADC read
    uint16_t raw = 1024U + channel * 100U;
    return Result<uint16_t>::ok(raw);
}

int main(void)
{
    // Zephyr-style error code checking
    int ret = configure_sensor(2, 100);
    if (ret < 0) {
        LOG_ERR("configure_sensor failed: %d (%s)", ret, strerror(-ret));
        return ret;
    }

    // Invalid config
    ret = configure_sensor(10, 100);
    LOG_INF("Invalid sensor ID returned: %d (%s)", ret, strerror(-ret));

    // Result type usage
    for (uint8_t ch = 0; ch < 3; ch++) {
        auto result = read_adc(ch);
        if (result.is_ok()) {
            LOG_INF("ADC[%u] = %u", ch, result.value);
        } else {
            LOG_ERR("ADC[%u] read failed: %d", ch, result.err);
        }
    }

    // __ASSERT: debug check (stripped in release build with CONFIG_ASSERT=n)
    uint8_t buf[32];
    size_t len = sizeof(buf);
    __ASSERT(len > 0, "Buffer length must be positive");

    // static_assert with template parameter
    constexpr size_t QUEUE_DEPTH = 16;
    static_assert((QUEUE_DEPTH & (QUEUE_DEPTH - 1)) == 0, "Queue depth must be power of 2");

    LOG_INF("All error handling checks passed");

    while (1) { k_msleep(5000); }
    return 0;
}
