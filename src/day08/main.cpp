#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(day08, LOG_LEVEL_INF);

static inline uint32_t get_msp(void) {
    uint32_t v; __asm volatile("MRS %0, MSP" : "=r"(v)); return v;
}
static inline uint32_t get_psp(void) {
    uint32_t v; __asm volatile("MRS %0, PSP" : "=r"(v)); return v;
}

int main(void)
{
    LOG_INF("=== ARM Cortex-M4 (nRF52840) ===");
    LOG_INF("MSP: 0x%08X", get_msp());
    LOG_INF("PSP: 0x%08X", get_psp());
    LOG_INF("SysTick LOAD: %u", SysTick->LOAD);
    LOG_INF("GPIOTE IRQ prio: %u", NVIC_GetPriority(GPIOTE_IRQn));
    LOG_INF("Flash @ 0x%08X  RAM @ 0x%08X", 0x00000000, 0x20000000);
    while (1) { k_msleep(5000); }
    return 0;
}
