#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(day10, LOG_LEVEL_INF);

static void mmio_demo(void) {
    static volatile uint32_t fake_reg = 0UL;
    volatile uint32_t *p = &fake_reg;
    *p = 0xDEAD1234UL;
    LOG_INF("MMIO read: 0x%08X", *p);
}

static void const_ptr_demo(void) {
    uint8_t buf[4] = {10, 20, 30, 40};
    const uint8_t *p_data = buf;          // pointer to const data
    uint8_t * const c_ptr = buf;          // const pointer
    *c_ptr = 99;
    LOG_INF("p_data[0]=%u  c_ptr->buf[0]=%u", *p_data, buf[0]);
}

static void ref_demo(volatile uint32_t &r) { r++; }

extern "C" void c_cb(void *ctx) {
    LOG_INF("C callback: %s", (const char *)ctx);
}

int main(void)
{
    mmio_demo();
    const_ptr_demo();
    volatile uint32_t cnt = 0;
    ref_demo(cnt); ref_demo(cnt);
    LOG_INF("Counter: %u", cnt);
    uint8_t *np = nullptr;
    LOG_INF("nullptr check: %s", np == nullptr ? "null" : "not null");
    static const char msg[] = "hello";
    c_cb((void *)msg);
    while (1) { k_msleep(5000); }
    return 0;
}
