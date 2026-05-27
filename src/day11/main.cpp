#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(day11_structs, LOG_LEVEL_INF);

// Register-mapped struct using bit-fields (nRF52840 GPIO PIN_CNF layout)
struct __packed gpio_pin_cnf {
    uint32_t dir       : 1;   // bit 0: direction (0=in, 1=out)
    uint32_t input     : 1;   // bit 1: input buffer (0=connect, 1=disconnect)
    uint32_t pull      : 2;   // bits 3:2: pull config
    uint32_t reserved1 : 4;   // bits 7:4: reserved
    uint32_t drive     : 3;   // bits 10:8: drive config
    uint32_t reserved2 : 5;   // bits 15:11: reserved
    uint32_t sense     : 2;   // bits 17:16: pin sensing
    uint32_t reserved3 : 14;  // bits 31:18: reserved
};

// Union for type punning: same memory, two views
union float_bytes {
    float    f;
    uint8_t  bytes[4];
    uint32_t raw;
};

// Sensor data struct (practical example)
struct sensor_reading {
    uint32_t timestamp_ms;
    int16_t  temperature_cdeg;   // hundredths of degree C
    uint16_t humidity_pcent;     // 0-10000 = 0.00-100.00%
    uint8_t  sensor_id;
    bool     valid;
} __packed;

int main(void)
{
    // Bit-field struct usage
    struct gpio_pin_cnf cfg = {};
    cfg.dir   = 1;   // output
    cfg.input = 1;   // disconnect input buffer
    cfg.drive = 0;   // standard 0H1H
    cfg.pull  = 0;   // no pull

    uint32_t raw_cnf;
    memcpy(&raw_cnf, &cfg, sizeof(raw_cnf));
    LOG_INF("PIN_CNF raw value: 0x%08X", raw_cnf);

    // Union type punning: inspect float bytes
    union float_bytes fb;
    fb.f = 3.14159f;
    LOG_INF("float 3.14159 bytes: %02X %02X %02X %02X",
            fb.bytes[0], fb.bytes[1], fb.bytes[2], fb.bytes[3]);
    LOG_INF("float 3.14159 raw uint32: 0x%08X", fb.raw);

    // Sensor reading struct
    struct sensor_reading reading = {
        .timestamp_ms    = k_uptime_get_32(),
        .temperature_cdeg = 2350,   // 23.50 C
        .humidity_pcent   = 6512,   // 65.12%
        .sensor_id        = 0x01,
        .valid            = true,
    };
    LOG_INF("Sensor[%u] @ %ums: temp=%d.%02d C, humidity=%u.%02u%%",
            reading.sensor_id, reading.timestamp_ms,
            reading.temperature_cdeg / 100, abs(reading.temperature_cdeg % 100),
            reading.humidity_pcent / 100, reading.humidity_pcent % 100);
    LOG_INF("Struct size: %zu bytes (packed)", sizeof(struct sensor_reading));

    while (1) { k_msleep(5000); }
    return 0;
}
