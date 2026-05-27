#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/smf.h>
#include <string.h>
#include <stdlib.h>

LOG_MODULE_REGISTER(capstone, LOG_LEVEL_INF);

// ── LED Manager ───────────────────────────────────────────────────────────
class LedManager {
public:
    static LedManager &instance() {
        static LedManager inst;
        return inst;
    }
    bool init() {
        bool ok = true;
        for (size_t i = 0; i < ARRAY_SIZE(leds_); i++) {
            if (gpio_is_ready_dt(&leds_[i])) {
                gpio_pin_configure_dt(&leds_[i], GPIO_OUTPUT_INACTIVE);
            } else { ok = false; }
        }
        return ok;
    }
    void set(uint8_t idx, bool on) {
        if (idx < ARRAY_SIZE(leds_)) {
            gpio_pin_set_dt(&leds_[idx], on ? 1 : 0);
            blink_[idx] = false;
        }
    }
    void blink(uint8_t idx) {
        if (idx < ARRAY_SIZE(leds_)) blink_[idx] = true;
    }
    void tick() {
        for (size_t i = 0; i < ARRAY_SIZE(leds_); i++) {
            if (blink_[i]) gpio_pin_toggle_dt(&leds_[i]);
        }
    }
private:
    LedManager() = default;
    static const struct gpio_dt_spec leds_[4];
    bool blink_[4] = {};
};
const struct gpio_dt_spec LedManager::leds_[4] = {
    GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios),
    GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios),
    GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios),
    GPIO_DT_SPEC_GET(DT_ALIAS(led3), gpios),
};

// ── UART Shell ─────────────────────────────────────────────────────────────
static const struct device *uart = DEVICE_DT_GET(DT_NODELABEL(uart0));
static const struct device *temp_dev = DEVICE_DT_GET(DT_NODELABEL(temp));

static char rx_buf[64];
static volatile size_t rx_head = 0;
K_SEM_DEFINE(cmd_ready, 0, 1);
K_MUTEX_DEFINE(uart_tx_mutex);
static uint32_t error_count = 0;

static void uart_tx(const char *msg)
{
    k_mutex_lock(&uart_tx_mutex, K_FOREVER);
    for (const char *c = msg; *c; c++) uart_poll_out(uart, *c);
    k_mutex_unlock(&uart_tx_mutex);
}

static void uart_cb(const struct device *dev, void *user_data)
{
    ARG_UNUSED(user_data);
    if (!uart_irq_update(dev) || !uart_irq_rx_ready(dev)) return;
    uint8_t ch;
    while (uart_fifo_read(dev, &ch, 1) == 1) {
        if (rx_head < sizeof(rx_buf) - 1) rx_buf[rx_head++] = ch;
        if (ch == '\n' || ch == '\r') {
            rx_buf[rx_head] = '\0';
            k_sem_give(&cmd_ready);
        }
    }
}

static void handle_command(char *cmd)
{
    char *tok = strtok(cmd, " \r\n");
    if (!tok) return;

    if (strcmp(tok, "help") == 0) {
        uart_tx("Commands: led <on|off|blink> <0-3>, temp, status, help\r\n");

    } else if (strcmp(tok, "temp") == 0) {
        struct sensor_value sv;
        if (sensor_sample_fetch(temp_dev) == 0 &&
            sensor_channel_get(temp_dev, SENSOR_CHAN_DIE_TEMP, &sv) == 0) {
            char buf[32];
            snprintk(buf, sizeof(buf), "Temp: %d.%02d C\r\n",
                     sv.val1, abs(sv.val2 / 10000));
            uart_tx(buf);
        } else {
            uart_tx("Temp read failed\r\n");
            error_count++;
        }

    } else if (strcmp(tok, "status") == 0) {
        char buf[64];
        snprintk(buf, sizeof(buf), "Uptime: %u ms, Errors: %u\r\n",
                 k_uptime_get_32(), error_count);
        uart_tx(buf);

    } else if (strcmp(tok, "led") == 0) {
        char *action = strtok(NULL, " \r\n");
        char *idx_s  = strtok(NULL, " \r\n");
        if (!action || !idx_s) { uart_tx("Usage: led <on|off|blink> <0-3>\r\n"); return; }
        uint8_t idx = (uint8_t)atoi(idx_s);
        if (strcmp(action, "on")    == 0) LedManager::instance().set(idx, true);
        else if (strcmp(action, "off")   == 0) LedManager::instance().set(idx, false);
        else if (strcmp(action, "blink") == 0) LedManager::instance().blink(idx);
        else { uart_tx("Unknown LED action\r\n"); return; }
        uart_tx("OK\r\n");

    } else {
        uart_tx("Unknown command. Type 'help'\r\n");
        error_count++;
    }
}

// ── Shell thread ──────────────────────────────────────────────────────────
static void shell_thread(void *p1, void *p2, void *p3)
{
    ARG_UNUSED(p1); ARG_UNUSED(p2); ARG_UNUSED(p3);
    uart_tx("\r\n=== nRF52840 Embedded C++ Shell ===\r\n");
    uart_tx("Type 'help' for commands\r\n> ");

    while (1) {
        if (k_sem_take(&cmd_ready, K_MSEC(100)) == 0) {
            // Copy and reset buffer atomically
            unsigned int key = irq_lock();
            char cmd[64];
            strncpy(cmd, rx_buf, sizeof(cmd));
            rx_head = 0;
            irq_unlock(key);

            handle_command(cmd);
            uart_tx("> ");
        }
    }
}

// ── Blink tick thread ─────────────────────────────────────────────────────
static void blink_thread(void *p1, void *p2, void *p3)
{
    ARG_UNUSED(p1); ARG_UNUSED(p2); ARG_UNUSED(p3);
    while (1) {
        LedManager::instance().tick();
        k_msleep(500);
    }
}

K_THREAD_DEFINE(shell_tid, 1024, shell_thread, NULL, NULL, NULL, 5, 0, 0);
K_THREAD_DEFINE(blink_tid, 512,  blink_thread, NULL, NULL, NULL, 8, 0, 0);

int main(void)
{
    LedManager::instance().init();

    if (!device_is_ready(uart)) { return -1; }
    uart_irq_callback_user_data_set(uart, uart_cb, nullptr);
    uart_irq_rx_enable(uart);

    if (!device_is_ready(temp_dev)) {
        LOG_WRN("Temp sensor not ready");
    }

    LOG_INF("Capstone application started");

    while (1) {
        LOG_DBG("Main heartbeat: %u ms uptime", k_uptime_get_32());
        k_msleep(30000);
    }
    return 0;
}
