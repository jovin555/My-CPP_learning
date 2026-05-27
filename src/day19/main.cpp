#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(day19_operators, LOG_LEVEL_INF);

// Strong typedef: a PinNumber that cannot be accidentally used as a raw int
class PinNumber {
public:
    explicit PinNumber(uint8_t n) : n_(n) {
        __ASSERT(n < 32, "Invalid pin number");
    }
    uint8_t get() const { return n_; }
    bool operator==(const PinNumber &o) const { return n_ == o.n_; }
    bool operator!=(const PinNumber &o) const { return n_ != o.n_; }
private:
    uint8_t n_;
};

// Register wrapper with overloaded operators
class Reg32 {
public:
    explicit Reg32(volatile uint32_t *addr) : reg_(addr) {}

    Reg32 &operator|=(uint32_t mask)  { *reg_ |=  mask; return *this; }
    Reg32 &operator&=(uint32_t mask)  { *reg_ &=  mask; return *this; }
    Reg32 &operator^=(uint32_t mask)  { *reg_ ^=  mask; return *this; }
    Reg32 &operator=(uint32_t val)    { *reg_  =  val;  return *this; }
    operator uint32_t() const         { return *reg_; }

    void set_bit(uint8_t bit)         { *reg_ |=  (1UL << bit); }
    void clr_bit(uint8_t bit)         { *reg_ &= ~(1UL << bit); }
    bool tst_bit(uint8_t bit) const   { return (*reg_ >> bit) & 1U; }

private:
    volatile uint32_t *reg_;
};

// Type-safe millisecond duration (prevents mixing ms and ticks)
class Milliseconds {
public:
    explicit constexpr Milliseconds(uint32_t v) : v_(v) {}
    uint32_t value() const { return v_; }
    Milliseconds operator+(Milliseconds o) const { return Milliseconds(v_ + o.v_); }
private:
    uint32_t v_;
};

static constexpr Milliseconds operator ""_ms(unsigned long long v) {
    return Milliseconds(static_cast<uint32_t>(v));
}

int main(void)
{
    // Register wrapper usage
    static volatile uint32_t fake_gpio_out = 0UL;
    Reg32 gpio_out(&fake_gpio_out);

    gpio_out = 0x00000000UL;
    gpio_out |= BIT(3);          // Set pin 3
    gpio_out |= BIT(7);          // Set pin 7
    LOG_INF("After set 3,7: 0x%08X", (uint32_t)gpio_out);

    gpio_out &= ~BIT(3);         // Clear pin 3
    LOG_INF("After clr 3:   0x%08X", (uint32_t)gpio_out);

    gpio_out.set_bit(0);
    LOG_INF("bit 0 set:     %s", gpio_out.tst_bit(0) ? "yes" : "no");

    // Strong typedef usage
    PinNumber led_pin(13U);
    PinNumber btn_pin(11U);
    LOG_INF("LED pin: %u, BTN pin: %u", led_pin.get(), btn_pin.get());
    LOG_INF("Same pin? %s", (led_pin == btn_pin) ? "yes" : "no");

    // Type-safe duration
    constexpr Milliseconds blink_period = 500_ms;
    constexpr Milliseconds debounce     = 50_ms;
    constexpr Milliseconds total        = blink_period + debounce;
    LOG_INF("Total duration: %u ms", total.value());

    k_msleep(blink_period.value());

    while (1) { k_msleep(5000); }
    return 0;
}
