#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/logging/log.h>
#include <string.h>

LOG_MODULE_REGISTER(day14_gpio_uart, LOG_LEVEL_INF);

// GPIO: 4 LEDs on nRF52840DK
static const struct gpio_dt_spec leds[] = {
    GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios),
    GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios),
    GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios),
    GPIO_DT_SPEC_GET(DT_ALIAS(led3), gpios),
};

// UART: nRF52840DK has UART0 connected to USB/JLink CDC
static const struct device *uart_dev = DEVICE_DT_GET(DT_NODELABEL(uart0));

// UART RX ring buffer
static uint8_t uart_rx_buf[64];
static volatile size_t uart_rx_head = 0;
K_SEM_DEFINE(uart_data_ready, 0, 1);

static void uart_cb(const struct device *dev, void *user_data)
{
    ARG_UNUSED(user_data);
    if (!uart_irq_update(dev) || !uart_irq_rx_ready(dev)) return;

    uint8_t ch;
    while (uart_fifo_read(dev, &ch, 1) == 1) {
        if (uart_rx_head < sizeof(uart_rx_buf) - 1) {
            uart_rx_buf[uart_rx_head++] = ch;
        }
        if (ch == '\n' || ch == '\r') {
            k_sem_give(&uart_data_ready);
        }
    }
}

static void init_leds(void)
{
    for (size_t i = 0; i < ARRAY_SIZE(leds); i++) {
        if (gpio_is_ready_dt(&leds[i])) {
            gpio_pin_configure_dt(&leds[i], GPIO_OUTPUT_INACTIVE);
        }
    }
}

static void blink_pattern(uint8_t pattern)
{
    for (size_t i = 0; i < ARRAY_SIZE(leds); i++) {
        gpio_pin_set_dt(&leds[i], (pattern >> i) & 1);
    }
}

int main(void)
{
    init_leds();

    if (!device_is_ready(uart_dev)) {
        LOG_ERR("UART not ready");
        return -1;
    }
    uart_irq_callback_user_data_set(uart_dev, uart_cb, nullptr);
    uart_irq_rx_enable(uart_dev);

    LOG_INF("Type a number 0-15 + Enter to control LEDs");

    uint8_t pattern = 0;
    while (1) {
        blink_pattern(pattern);

        if (k_sem_take(&uart_data_ready, K_MSEC(1000)) == 0) {
            uart_rx_buf[uart_rx_head] = '\0';
            pattern = (uint8_t)atoi((char *)uart_rx_buf) & 0x0F;
            LOG_INF("LED pattern: 0x%X", pattern);
            uart_rx_head = 0;
        } else {
            // Cycle pattern automatically if no input
            pattern = (pattern + 1) & 0x0F;
        }
    }
    return 0;
}
