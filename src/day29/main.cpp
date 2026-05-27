#include <zephyr/kernel.h>
#include <zephyr/pm/pm.h>
#include <zephyr/pm/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(day29_power, LOG_LEVEL_INF);

// Power state notifier callback
static int pm_notifier_cb(const struct pm_notifier *notifier,
                           enum pm_state state)
{
    switch (state) {
    case PM_STATE_RUNTIME_IDLE:
        LOG_DBG("PM: entering runtime idle");
        break;
    case PM_STATE_SUSPEND_TO_IDLE:
        LOG_DBG("PM: entering suspend to idle");
        break;
    case PM_STATE_STANDBY:
        LOG_DBG("PM: entering standby");
        break;
    default:
        break;
    }
    return 0;
}

static struct pm_notifier pm_notifier = {
    .state_entry = pm_notifier_cb,
};

// Suspend a peripheral when not in use (saves power)
static const struct device *uart_dev = DEVICE_DT_GET(DT_NODELABEL(uart0));

static void suspend_uart(void)
{
    int ret = pm_device_action_run(uart_dev, PM_DEVICE_ACTION_SUSPEND);
    if (ret == 0) {
        LOG_INF("UART suspended (power save)");
    } else if (ret == -ENOTSUP) {
        LOG_INF("UART PM suspend not supported");
    }
}

static void resume_uart(void)
{
    int ret = pm_device_action_run(uart_dev, PM_DEVICE_ACTION_RESUME);
    if (ret == 0) {
        LOG_INF("UART resumed");
    }
}

// Demonstrate WFE (Wait For Event) — ARM instruction for low-power wait
static inline void cpu_wfe(void)
{
    __asm volatile("wfe");
}

int main(void)
{
    // Register PM notifier
    pm_notifier_register(&pm_notifier);

    LOG_INF("Power management demo on nRF52840");
    LOG_INF("Zephyr PM enabled: %s", IS_ENABLED(CONFIG_PM) ? "yes" : "no");
    LOG_INF("Device PM enabled: %s", IS_ENABLED(CONFIG_PM_DEVICE) ? "yes" : "no");

    // Active work for 3 seconds
    for (int i = 0; i < 3; i++) {
        LOG_INF("Active work iteration %d", i);
        k_msleep(1000);
    }

    // Suspend UART to save power during idle
    suspend_uart();
    LOG_INF("Entering low-activity period (5s)...");
    k_msleep(5000);  // Zephyr will enter idle automatically between ticks

    // Resume
    resume_uart();
    LOG_INF("Back to active mode");

    // Show current PM state
    enum pm_state current = pm_state_force(0U, NULL);
    (void)current;
    LOG_INF("System active. Use Nordic PPK2 to measure current draw.");

    while (1) {
        LOG_INF("Heartbeat - uptime: %u ms", k_uptime_get_32());
        k_msleep(10000);  // Long sleep — CPU enters idle automatically
    }
    return 0;
}
