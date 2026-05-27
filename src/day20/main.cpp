#include <zephyr/kernel.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/logging/log.h>
#include <cstring>

LOG_MODULE_REGISTER(day20_singleton, LOG_LEVEL_INF);

// Thread-safe UART manager singleton
class UartManager {
public:
    // Meyers Singleton: thread-safe in C++11, initialized on first call
    static UartManager &instance() {
        static UartManager inst;
        return inst;
    }

    bool init(const struct device *dev) {
        if (initialized_) return true;
        if (!device_is_ready(dev)) return false;
        dev_ = dev;
        initialized_ = true;
        LOG_INF("UartManager singleton initialized");
        return true;
    }

    bool send(const char *msg) {
        if (!initialized_) return false;
        k_mutex_lock(&mutex_, K_FOREVER);
        for (const char *c = msg; *c; c++) {
            uart_poll_out(dev_, *c);
        }
        k_mutex_unlock(&mutex_);
        return true;
    }

    bool is_ready() const { return initialized_; }

    // Prevent copy/move
    UartManager(const UartManager &)            = delete;
    UartManager &operator=(const UartManager &) = delete;

private:
    UartManager() : dev_(nullptr), initialized_(false) {
        k_mutex_init(&mutex_);
    }

    const struct device *dev_;
    bool initialized_;
    struct k_mutex mutex_;
};

// Monostate pattern: all state is static (alternative to singleton)
class SystemStatus {
public:
    static void set_error(uint32_t code)  { error_code_ = code;  }
    static uint32_t get_error()           { return error_code_;  }
    static void set_uptime(uint32_t ms)   { uptime_ms_ = ms;     }
    static uint32_t get_uptime()          { return uptime_ms_;   }
private:
    static uint32_t error_code_;
    static uint32_t uptime_ms_;
};
uint32_t SystemStatus::error_code_ = 0;
uint32_t SystemStatus::uptime_ms_  = 0;

int main(void)
{
    const struct device *uart = DEVICE_DT_GET(DT_NODELABEL(uart0));
    UartManager::instance().init(uart);
    UartManager::instance().send("Singleton UART ready\r\n");

    // Prove it's the same instance
    UartManager &a = UartManager::instance();
    UartManager &b = UartManager::instance();
    LOG_INF("Same instance? %s", (&a == &b) ? "yes" : "no");

    // Monostate
    SystemStatus::set_uptime(k_uptime_get_32());
    SystemStatus::set_error(0);
    LOG_INF("System uptime: %u ms, error: %u",
            SystemStatus::get_uptime(), SystemStatus::get_error());

    while (1) {
        SystemStatus::set_uptime(k_uptime_get_32());
        k_msleep(1000);
    }
    return 0;
}
