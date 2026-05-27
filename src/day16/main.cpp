#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(day16_raii, LOG_LEVEL_INF);

// RAII mutex lock guard for Zephyr k_mutex
class ScopedLock {
public:
    explicit ScopedLock(struct k_mutex &mtx) : mtx_(mtx), locked_(false) {
        if (k_mutex_lock(&mtx_, K_FOREVER) == 0) {
            locked_ = true;
        }
    }
    ~ScopedLock() {
        if (locked_) {
            k_mutex_unlock(&mtx_);
        }
    }
    bool is_locked() const { return locked_; }

    // Prevent copying (hardware resource — only one owner)
    ScopedLock(const ScopedLock &)            = delete;
    ScopedLock &operator=(const ScopedLock &) = delete;

private:
    struct k_mutex &mtx_;
    bool locked_;
};

// RAII peripheral manager (deferred init pattern)
class UartManager {
public:
    UartManager() : dev_(nullptr), initialized_(false) {}  // Construct without init

    bool init(const struct device *dev) {
        if (!device_is_ready(dev)) return false;
        dev_ = dev;
        initialized_ = true;
        LOG_INF("UartManager: initialized");
        return true;
    }

    ~UartManager() {
        if (initialized_) {
            // Would disable UART, free DMA, etc.
            LOG_INF("UartManager: cleaned up");
        }
    }

    bool send(const uint8_t *data, size_t len) {
        if (!initialized_) return false;
        for (size_t i = 0; i < len; i++) {
            // uart_poll_out(dev_, data[i]);  // Real usage
            (void)data[i];
        }
        return true;
    }

    UartManager(const UartManager &)            = delete;
    UartManager &operator=(const UartManager &) = delete;

private:
    const struct device *dev_;
    bool initialized_;
};

K_MUTEX_DEFINE(shared_resource_mutex);
static uint32_t shared_counter = 0;

static void increment_safely(void)
{
    ScopedLock lock(shared_resource_mutex);  // Locks on construction
    if (lock.is_locked()) {
        shared_counter++;
        LOG_INF("Counter incremented to: %u", shared_counter);
    }
    // Lock released automatically here (destructor called)
}

int main(void)
{
    increment_safely();
    increment_safely();
    increment_safely();
    LOG_INF("Final counter: %u", shared_counter);

    // Deferred init pattern
    UartManager uart;
    const struct device *uart_dev = DEVICE_DT_GET(DT_NODELABEL(uart0));
    if (!uart.init(uart_dev)) {
        LOG_ERR("UART init failed");
    }

    while (1) { k_msleep(5000); }
    return 0;
}
