#include <zephyr/kernel.h>
#include <stdint.h>
#include <stdbool.h>

static volatile bool isr_flag = false;
static const uint8_t MAX_ITEMS = 10U;
static constexpr uint32_t BLINK_MS = 500U;
static uint32_t counter = 0U;

int main(void)
{
    printk("uint8_t  = %zu bytes\n", sizeof(uint8_t));
    printk("uint16_t = %zu bytes\n", sizeof(uint16_t));
    printk("uint32_t = %zu bytes\n", sizeof(uint32_t));
    printk("uint64_t = %zu bytes\n", sizeof(uint64_t));
    printk("pointer  = %zu bytes\n", sizeof(void *));
    printk("MAX_ITEMS  = %u\n", MAX_ITEMS);
    printk("BLINK_MS   = %u\n", BLINK_MS);
    counter++;
    printk("counter    = %u\n", counter);
    printk("isr_flag   = %s\n", isr_flag ? "true" : "false");
    while (1) { k_msleep(5000); }
    return 0;
}
