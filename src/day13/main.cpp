#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(day13_isr, LOG_LEVEL_INF);

// Semaphore to signal from ISR to main thread (safe pattern)
K_SEM_DEFINE(button_sem, 0, 1);

// volatile: compiler cannot optimize away reads/writes to this
static volatile uint32_t isr_event_count = 0;

static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);
static struct gpio_callback button_cb;

// ISR — keep it SHORT: set flag, give semaphore, return
static void button_isr(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    ARG_UNUSED(dev); ARG_UNUSED(cb); ARG_UNUSED(pins);

    isr_event_count++;          // Atomic on single-core, volatile is enough here
    k_sem_give(&button_sem);    // Wake up main thread — this is ISR-safe in Zephyr
}

// Direct ISR example (minimal latency, no Zephyr overhead)
// This would be used for timing-critical peripherals
static void demonstrate_irq_lock(void)
{
    // Critical section: disable all interrupts temporarily
    unsigned int key = irq_lock();

    // DANGER ZONE: no interrupts here, keep VERY short
    volatile uint32_t snapshot = isr_event_count;  // Atomic read
    // ... other critical operations ...

    irq_unlock(key);  // Re-enable interrupts

    LOG_INF("Snapshot of isr_event_count: %u", snapshot);
}

int main(void)
{
    if (!gpio_is_ready_dt(&button)) {
        LOG_ERR("Button GPIO not ready");
        return -1;
    }

    gpio_pin_configure_dt(&button, GPIO_INPUT);
    gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
    gpio_init_callback(&button_cb, button_isr, BIT(button.pin));
    gpio_add_callback(button.port, &button_cb);

    LOG_INF("Waiting for button presses (SW0 on nRF52840DK)...");

    while (1) {
        // Block here until ISR gives the semaphore
        if (k_sem_take(&button_sem, K_MSEC(5000)) == 0) {
            LOG_INF("Button event #%u detected!", isr_event_count);
            demonstrate_irq_lock();
        } else {
            LOG_INF("No button press for 5s (events so far: %u)", isr_event_count);
        }
    }
    return 0;
}
