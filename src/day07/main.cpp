#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(day07, LOG_LEVEL_DBG);

#define APP_CHK(cond, msg) do { if (!(cond)) { LOG_ERR("FAIL: " msg); k_oops(); } } while(0)

static int safe_div(int a, int b) {
    __ASSERT(b != 0, "divide by zero");
    return a / b;
}

int main(void)
{
    LOG_DBG("debug msg");
    LOG_INF("info msg");
    LOG_WRN("warning msg");
    LOG_ERR("error msg (non-fatal demo)");

    int r = safe_div(20, 4);
    LOG_INF("20/4 = %d", r);

    uint8_t buf[8] = {0};
    APP_CHK(sizeof(buf) == 8, "buffer size mismatch");
    buf[0] = 0xFF;
    LOG_INF("buf[0] = 0x%02X", buf[0]);

    while (1) { k_msleep(5000); }
    return 0;
}
