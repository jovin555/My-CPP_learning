#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(day17_inheritance, LOG_LEVEL_INF);

// Abstract sensor interface
class ISensor {
public:
    virtual ~ISensor() = default;
    virtual bool init()                      = 0;
    virtual bool read(int32_t *out_value)    = 0;
    virtual const char *name() const         = 0;
};

// Concrete: simulated temperature sensor
class FakeTemperatureSensor : public ISensor {
public:
    bool init() override {
        LOG_INF("[%s] init OK", name());
        return true;
    }
    bool read(int32_t *out_value) override {
        *out_value = 2350 + (k_uptime_get_32() % 100);  // ~23.50 C
        return true;
    }
    const char *name() const override { return "FakeTempSensor"; }
};

// Concrete: simulated humidity sensor
class FakeHumiditySensor : public ISensor {
public:
    bool init() override {
        LOG_INF("[%s] init OK", name());
        return true;
    }
    bool read(int32_t *out_value) override {
        *out_value = 6500 + (k_uptime_get_32() % 200);  // ~65%
        return true;
    }
    const char *name() const override { return "FakeHumSensor"; }
};

// HAL: works with any ISensor without knowing its type
static void poll_sensor(ISensor &sensor)
{
    int32_t val;
    if (sensor.read(&val)) {
        LOG_INF("[%s] value: %d", sensor.name(), val);
    } else {
        LOG_ERR("[%s] read failed", sensor.name());
    }
}

int main(void)
{
    FakeTemperatureSensor temp_sensor;
    FakeHumiditySensor    hum_sensor;

    // Array of pointers to base class — runtime polymorphism
    ISensor *sensors[] = { &temp_sensor, &hum_sensor };

    for (auto *s : sensors) {
        s->init();
    }

    for (int i = 0; i < 3; i++) {
        for (auto *s : sensors) {
            poll_sensor(*s);
        }
        k_msleep(1000);
    }

    LOG_INF("vtable ptr size per object: %zu bytes", sizeof(void *));
    LOG_INF("FakeTempSensor size: %zu bytes", sizeof(FakeTemperatureSensor));

    while (1) { k_msleep(5000); }
    return 0;
}
