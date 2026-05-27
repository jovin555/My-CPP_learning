#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(day27_threads, LOG_LEVEL_INF);

// Shared data between threads
static volatile uint32_t shared_counter = 0;
K_MUTEX_DEFINE(counter_mutex);

// ── Thread 1: High priority sensor polling ────────────────────────────────
#define SENSOR_STACK_SIZE 512
#define SENSOR_PRIORITY   5

static void sensor_thread(void *p1, void *p2, void *p3)
{
    ARG_UNUSED(p1); ARG_UNUSED(p2); ARG_UNUSED(p3);
    LOG_INF("[sensor_thread] started, priority=%d", k_thread_priority_get(k_current_get()));

    while (1) {
        uint16_t fake_adc = (uint16_t)(k_uptime_get_32() & 0xFFF);

        k_mutex_lock(&counter_mutex, K_FOREVER);
        shared_counter++;
        k_mutex_unlock(&counter_mutex);

        LOG_INF("[sensor] ADC=%u counter=%u", fake_adc, shared_counter);
        k_msleep(500);
    }
}

// ── Thread 2: Lower priority display/logging ──────────────────────────────
#define DISPLAY_STACK_SIZE 512
#define DISPLAY_PRIORITY   10

static void display_thread(void *p1, void *p2, void *p3)
{
    ARG_UNUSED(p1); ARG_UNUSED(p2); ARG_UNUSED(p3);
    LOG_INF("[display_thread] started, priority=%d", k_thread_priority_get(k_current_get()));

    while (1) {
        uint32_t c;
        k_mutex_lock(&counter_mutex, K_FOREVER);
        c = shared_counter;
        k_mutex_unlock(&counter_mutex);

        LOG_INF("[display] counter snapshot: %u", c);
        k_msleep(1500);
    }
}

// Static thread definitions
K_THREAD_DEFINE(sensor_tid,  SENSOR_STACK_SIZE,  sensor_thread,  NULL, NULL, NULL, SENSOR_PRIORITY,  0, 0);
K_THREAD_DEFINE(display_tid, DISPLAY_STACK_SIZE, display_thread, NULL, NULL, NULL, DISPLAY_PRIORITY, 0, 0);

int main(void)
{
    LOG_INF("Main thread priority: %d", k_thread_priority_get(k_current_get()));
    LOG_INF("Scheduler: preemptive (Zephyr default)");

    // Show thread names
    LOG_INF("sensor_thread  stack: %u bytes", SENSOR_STACK_SIZE);
    LOG_INF("display_thread stack: %u bytes", DISPLAY_STACK_SIZE);

    // Main thread just monitors
    for (int i = 0; i < 10; i++) {
        LOG_INF("[main] uptime: %u ms, counter: %u",
                k_uptime_get_32(), shared_counter);
        k_msleep(1000);
    }

    LOG_INF("[main] done monitoring");
    while (1) { k_msleep(10000); }
    return 0;
}
