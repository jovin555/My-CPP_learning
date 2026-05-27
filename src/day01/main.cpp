#include <zephyr/kernel.h>

int main(void)
{
    printk("Hello from nRF52840 + Zephyr!\n");
    while (1) {
        printk("Tick: %u ms\n", k_uptime_get_32());
        k_msleep(1000);
    }
    return 0;
}
