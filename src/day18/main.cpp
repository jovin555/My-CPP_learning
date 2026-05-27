#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <array>

LOG_MODULE_REGISTER(day18_templates, LOG_LEVEL_INF);

// Function template: works with any numeric type
template <typename T>
constexpr T map_range(T val, T in_min, T in_max, T out_min, T out_max)
{
    return out_min + (val - in_min) * (out_max - out_min) / (in_max - in_min);
}

// Class template: generic circular buffer, size checked at compile time
template <typename T, size_t N>
class CircularBuffer {
    static_assert(N > 0, "Buffer size must be > 0");
    static_assert((N & (N - 1)) == 0, "Buffer size must be power of 2 for fast modulo");
public:
    bool push(const T &v) {
        if (full()) return false;
        buf_[head_++ & (N-1)] = v;
        count_++;
        return true;
    }
    bool pop(T &v) {
        if (empty()) return false;
        v = buf_[tail_++ & (N-1)];
        count_--;
        return true;
    }
    bool empty() const { return count_ == 0; }
    bool full()  const { return count_ == N; }
private:
    std::array<T, N> buf_{};
    size_t head_ = 0, tail_ = 0, count_ = 0;
};

// CRTP: static polymorphism (no vtable, no virtual call overhead)
template <typename Derived>
class SensorBase {
public:
    bool init()               { return static_cast<Derived *>(this)->do_init(); }
    bool read(int32_t *out)   { return static_cast<Derived *>(this)->do_read(out); }
    const char *name() const  { return static_cast<const Derived *>(this)->do_name(); }
};

class PressureSensor : public SensorBase<PressureSensor> {
public:
    bool do_init()               { LOG_INF("Pressure sensor init"); return true; }
    bool do_read(int32_t *out)   { *out = 101325; return true; }  // 1 atm in Pa
    const char *do_name() const  { return "PressureSensor"; }
};

// Template function working with any SensorBase derivative — resolved at compile time
template <typename S>
void sample_and_log(SensorBase<S> &sensor)
{
    int32_t val;
    if (sensor.read(&val)) {
        LOG_INF("[%s]: %d", sensor.name(), val);
    }
}

int main(void)
{
    // map_range: ADC 12-bit (0-4095) to voltage (0-3300 mV)
    uint16_t adc_raw = 2048U;
    uint32_t voltage = map_range<uint32_t>(adc_raw, 0, 4095, 0, 3300);
    LOG_INF("ADC %u -> %u mV", adc_raw, voltage);

    // Circular buffer (power-of-2 size enforced at compile time)
    CircularBuffer<uint16_t, 8> adc_buf;
    for (uint16_t i = 100; i < 106; i++) adc_buf.push(i);
    LOG_INF("Buffer full: %s", adc_buf.full() ? "yes" : "no");
    uint16_t v;
    while (adc_buf.pop(v)) { LOG_INF("  pop: %u", v); }

    // CRTP sensor
    PressureSensor ps;
    ps.init();
    sample_and_log(ps);  // No virtual call — resolved at compile time

    LOG_INF("PressureSensor size (no vtable): %zu bytes", sizeof(PressureSensor));

    while (1) { k_msleep(5000); }
    return 0;
}
