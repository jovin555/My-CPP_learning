#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(day28_sync, LOG_LEVEL_INF);

// ── C++ RAII wrappers for Zephyr sync primitives ──────────────────────────
class ZMutex {
public:
    ZMutex()  { k_mutex_init(&mtx_); }
    void lock()   { k_mutex_lock(&mtx_, K_FOREVER); }
    void unlock() { k_mutex_unlock(&mtx_); }
    struct k_mutex &raw() { return mtx_; }
private:
    struct k_mutex mtx_;
};

class ZScopedLock {
public:
    explicit ZScopedLock(ZMutex &m) : m_(m) { m_.lock(); }
    ~ZScopedLock() { m_.unlock(); }
    ZScopedLock(const ZScopedLock &) = delete;
private:
    ZMutex &m_;
};

// Shared resources
static ZMutex spi_bus_mutex;
static uint32_t spi_transfer_count = 0;

// Counting semaphore: signals data available
K_SEM_DEFINE(data_ready_sem, 0, 5);  // max 5 pending signals

// ── Producer thread (simulates sensor DMA complete) ───────────────────────
static void producer_thread(void *p1, void *p2, void *p3)
{
    ARG_UNUSED(p1); ARG_UNUSED(p2); ARG_UNUSED(p3);
    for (int i = 0; i < 5; i++) {
        k_msleep(300);
        LOG_INF("[producer] data ready #%d", i);
        k_sem_give(&data_ready_sem);
    }
}

// ── Consumer thread ───────────────────────────────────────────────────────
static void consumer_thread(void *p1, void *p2, void *p3)
{
    ARG_UNUSED(p1); ARG_UNUSED(p2); ARG_UNUSED(p3);
    for (int i = 0; i < 5; i++) {
        k_sem_take(&data_ready_sem, K_FOREVER);  // Block until data ready
        {
            ZScopedLock lock(spi_bus_mutex);  // RAII lock
            spi_transfer_count++;
            LOG_INF("[consumer] processing sample #%d via SPI (transfer #%u)",
                    i, spi_transfer_count);
            k_msleep(50);  // Simulate SPI transfer time
        }  // Lock released here
    }
    LOG_INF("[consumer] done. Total SPI transfers: %u", spi_transfer_count);
}

// k_poll: wait on multiple events simultaneously
K_SEM_DEFINE(poll_sem_a, 0, 1);
K_SEM_DEFINE(poll_sem_b, 0, 1);

static void poll_demo(void)
{
    struct k_poll_event events[] = {
        K_POLL_EVENT_STATIC_INITIALIZER(K_POLL_TYPE_SEM_AVAILABLE, K_POLL_MODE_AUTO_RESET, &poll_sem_a, 0),
        K_POLL_EVENT_STATIC_INITIALIZER(K_POLL_TYPE_SEM_AVAILABLE, K_POLL_MODE_AUTO_RESET, &poll_sem_b, 0),
    };

    k_sem_give(&poll_sem_b);  // Signal B only

    int rc = k_poll(events, ARRAY_SIZE(events), K_MSEC(100));
    if (rc == 0) {
        if (events[0].state == K_POLL_STATE_SEM_AVAILABLE) LOG_INF("k_poll: sem_a signaled");
        if (events[1].state == K_POLL_STATE_SEM_AVAILABLE) LOG_INF("k_poll: sem_b signaled");
    } else {
        LOG_INF("k_poll timed out");
    }
}

K_THREAD_DEFINE(prod_tid, 512, producer_thread, NULL, NULL, NULL, 7, 0, 0);
K_THREAD_DEFINE(cons_tid, 512, consumer_thread, NULL, NULL, NULL, 8, 0, 0);

int main(void)
{
    // Wait for producer/consumer to finish
    k_msleep(2500);
    poll_demo();
    LOG_INF("Final SPI transfer count: %u", spi_transfer_count);

    while (1) { k_msleep(5000); }
    return 0;
}
