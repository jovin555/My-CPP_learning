#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(day15_classes, LOG_LEVEL_INF);

// A clean embedded C++ class wrapping Zephyr GPIO LED
class Led {
public:
    explicit Led(const struct gpio_dt_spec *spec) : spec_(spec), state_(false) {}

    bool init() {
        if (!gpio_is_ready_dt(spec_)) return false;
        return gpio_pin_configure_dt(spec_, GPIO_OUTPUT_INACTIVE) == 0;
    }

    void on()     { gpio_pin_set_dt(spec_, 1); state_ = true;  }
    void off()    { gpio_pin_set_dt(spec_, 0); state_ = false; }
    void toggle() { gpio_pin_toggle_dt(spec_); state_ = !state_; }
    bool is_on() const { return state_; }

private:
    const struct gpio_dt_spec *spec_;
    bool state_;
};

// Verify class is small — no hidden overhead without virtuals
static_assert(sizeof(Led) <= 16, "Led class too large for embedded");

// Simple PID controller class (common embedded use case)
class PidController {
public:
    PidController(float kp, float ki, float kd)
        : kp_(kp), ki_(ki), kd_(kd), integral_(0.0f), prev_error_(0.0f) {}

    float compute(float setpoint, float measured, float dt_s) {
        float error = setpoint - measured;
        integral_  += error * dt_s;
        float derivative = (error - prev_error_) / dt_s;
        prev_error_ = error;
        return (kp_ * error) + (ki_ * integral_) + (kd_ * derivative);
    }

    void reset() { integral_ = 0.0f; prev_error_ = 0.0f; }

private:
    const float kp_, ki_, kd_;
    float integral_;
    float prev_error_;
};

static const struct gpio_dt_spec led_spec = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

int main(void)
{
    Led led(&led_spec);
    if (!led.init()) {
        LOG_ERR("LED init failed");
        return -1;
    }

    PidController pid(1.0f, 0.1f, 0.01f);
    float setpoint = 100.0f;
    float measured = 80.0f;

    for (int i = 0; i < 5; i++) {
        float output = pid.compute(setpoint, measured, 0.01f);
        measured += output * 0.1f;  // Simulate system response
        LOG_INF("PID step %d: measured=%.2f output=%.2f", i, (double)measured, (double)output);
        led.toggle();
        k_msleep(100);
    }

    LOG_INF("Led is_on: %s", led.is_on() ? "yes" : "no");

    while (1) {
        led.toggle();
        k_msleep(500);
    }
    return 0;
}
