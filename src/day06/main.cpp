#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

typedef void (*event_cb_t)(uint8_t event_id, void *ctx);

static inline uint32_t clamp32(uint32_t v, uint32_t lo, uint32_t hi) {
    return v < lo ? lo : v > hi ? hi : v;
}

static const struct gpio_dt_spec btn = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);
static struct gpio_callback btn_cb_data;
static event_cb_t user_cb = nullptr;
static void *user_ctx = nullptr;

static void gpio_isr(const struct device *d, struct gpio_callback *cb, uint32_t pins) {
    ARG_UNUSED(d); ARG_UNUSED(cb); ARG_UNUSED(pins);
    if (user_cb) user_cb(1, user_ctx);
}

static void my_event_handler(uint8_t id, void *ctx) {
    printk("Event %u from %s\n", id, (const char *)ctx);
}

int main(void)
{
    printk("clamp(300,0,255) = %u\n", clamp32(300, 0, 255));

    static const char tag[] = "button";
    user_cb  = my_event_handler;
    user_ctx = (void *)tag;

    if (gpio_is_ready_dt(&btn)) {
        gpio_pin_configure_dt(&btn, GPIO_INPUT);
        gpio_pin_interrupt_configure_dt(&btn, GPIO_INT_EDGE_TO_ACTIVE);
        gpio_init_callback(&btn_cb_data, gpio_isr, BIT(btn.pin));
        gpio_add_callback(btn.port, &btn_cb_data);
        printk("Button callback registered\n");
    }
    while (1) { k_msleep(1000); }
    return 0;
}
