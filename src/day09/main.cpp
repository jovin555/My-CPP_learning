#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(day09, LOG_LEVEL_INF);

static uint32_t g_data = 0xCAFEBABEUL;   // .data
static uint32_t g_bss;                    // .bss (zero-init)
static const uint8_t g_rom[4] = {1,2,3,4}; // .rodata
static uint32_t g_boot __attribute__((section(".noinit")));

int main(void)
{
    g_boot++;
    LOG_INF("Boot #%u", g_boot);
    LOG_INF(".data : 0x%08X @ 0x%08X", g_data, (uint32_t)&g_data);
    LOG_INF(".bss  : %u       @ 0x%08X", g_bss,  (uint32_t)&g_bss);
    LOG_INF(".rodata[2]: %u   @ 0x%08X", g_rom[2], (uint32_t)g_rom);
    LOG_INF("SRAM base: 0x%08X  size: %u KB",
            (uint32_t)CONFIG_SRAM_BASE_ADDRESS, CONFIG_SRAM_SIZE);
    while (1) { k_msleep(5000); }
    return 0;
}
