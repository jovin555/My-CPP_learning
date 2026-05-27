#include <zephyr/kernel.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(day21_driver_arch, LOG_LEVEL_INF);

// ── Layer 1: HAL (Hardware Abstraction Layer) ──────────────────────────────
class ITemperatureSensor {
public:
    virtual ~ITemperatureSensor() = default;
    virtual bool    init()                   = 0;
    virtual bool    sample()                 = 0;
    virtual int32_t get_temp_mdeg() const    = 0;  // millidegrees C
};

// ── Layer 2: Driver (device-specific implementation) ───────────────────────
// Uses Zephyr sensor API for a real sensor (e.g., nRF52840 internal temp sensor)
class NrfInternalTemp : public ITemperatureSensor {
public:
    explicit NrfInternalTemp(const struct device *dev) : dev_(dev), temp_mdeg_(0) {}

    bool init() override {
        if (!device_is_ready(dev_)) {
            LOG_ERR("Internal temp sensor not ready");
            return false;
        }
        LOG_INF("NrfInternalTemp: ready");
        return true;
    }

    bool sample() override {
        struct sensor_value sv;
        if (sensor_sample_fetch(dev_) < 0) return false;
        if (sensor_channel_get(dev_, SENSOR_CHAN_DIE_TEMP, &sv) < 0) return false;
        temp_mdeg_ = sv.val1 * 1000 + sv.val2 / 1000;
        return true;
    }

    int32_t get_temp_mdeg() const override { return temp_mdeg_; }

private:
    const struct device *dev_;
    int32_t temp_mdeg_;
};

// ── Layer 3: Application (works with HAL interface) ────────────────────────
class ThermalMonitor {
public:
    explicit ThermalMonitor(ITemperatureSensor &sensor, int32_t alarm_mdeg)
        : sensor_(sensor), alarm_mdeg_(alarm_mdeg), alarm_count_(0) {}

    void update() {
        if (!sensor_.sample()) {
            LOG_ERR("Sensor sample failed");
            return;
        }
        int32_t t = sensor_.get_temp_mdeg();
        LOG_INF("Temperature: %d.%03d C", t / 1000, abs(t % 1000));
        if (t > alarm_mdeg_) {
            alarm_count_++;
            LOG_WRN("THERMAL ALARM #%u! Temp: %d mdeg", alarm_count_, t);
        }
    }

    uint32_t alarm_count() const { return alarm_count_; }

private:
    ITemperatureSensor &sensor_;
    int32_t alarm_mdeg_;
    uint32_t alarm_count_;
};

int main(void)
{
    const struct device *temp_dev = DEVICE_DT_GET(DT_NODELABEL(temp));

    NrfInternalTemp nrf_temp(temp_dev);
    if (!nrf_temp.init()) {
        LOG_ERR("Temp sensor init failed");
        return -1;
    }

    // Application only knows about ITemperatureSensor — not NrfInternalTemp
    ThermalMonitor monitor(nrf_temp, 40000);  // Alarm at 40.000 C

    for (int i = 0; i < 10; i++) {
        monitor.update();
        k_msleep(1000);
    }

    LOG_INF("Total alarms: %u", monitor.alarm_count());

    while (1) { k_msleep(5000); }
    return 0;
}
