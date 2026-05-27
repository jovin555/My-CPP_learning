#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/ring_buffer.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(day26_ringbuf, LOG_LEVEL_INF);

// ── K_MSGQ: fixed-size message queue (ISR-safe) ───────────────────────────
struct adc_sample {
    uint32_t timestamp_ms;
    uint16_t value;
    uint8_t  channel;
};
K_MSGQ_DEFINE(adc_msgq, sizeof(struct adc_sample), 8, 4);

// Simulate ISR sending ADC data to main thread
static void simulate_adc_isr(uint8_t ch, uint16_t val)
{
    struct adc_sample s = {
        .timestamp_ms = k_uptime_get_32(),
        .value        = val,
        .channel      = ch,
    };
    // k_msgq_put is ISR-safe with K_NO_WAIT
    if (k_msgq_put(&adc_msgq, &s, K_NO_WAIT) != 0) {
        LOG_WRN("ADC queue full — sample dropped");
    }
}

// ── Zephyr ring_buf: byte-stream ring buffer (good for UART RX) ───────────
RING_BUF_DECLARE(uart_ring, 128);

static void simulate_uart_receive(const uint8_t *data, size_t len)
{
    uint32_t written = ring_buf_put(&uart_ring, data, len);
    if (written < len) {
        LOG_WRN("Ring buf overflow: dropped %zu bytes", len - written);
    }
}

int main(void)
{
    // Simulate ISR producing ADC samples
    for (uint8_t ch = 0; ch < 4; ch++) {
        simulate_adc_isr(ch, 1000U + ch * 250U);
    }
    LOG_INF("ADC queue has %u items", k_msgq_num_used_get(&adc_msgq));

    // Consumer: drain the queue
    struct adc_sample sample;
    while (k_msgq_get(&adc_msgq, &sample, K_NO_WAIT) == 0) {
        LOG_INF("ADC[%u] @ %ums = %u (%.2f mV)",
                sample.channel, sample.timestamp_ms, sample.value,
                (double)sample.value * 3300.0 / 4095.0);
    }

    // Simulate UART ring buffer
    static const uint8_t uart_data[] = "AT+NRF52840
OK
";
    simulate_uart_receive(uart_data, sizeof(uart_data) - 1);

    LOG_INF("Ring buf used: %u / %u bytes",
            ring_buf_size_get(&uart_ring), ring_buf_capacity_get(&uart_ring));

    uint8_t rx_buf[32];
    uint32_t read = ring_buf_get(&uart_ring, rx_buf, sizeof(rx_buf) - 1);
    rx_buf[read] = '\0';
    LOG_INF("Ring buf read: '%s'", rx_buf);

    while (1) { k_msleep(5000); }
    return 0;
}
