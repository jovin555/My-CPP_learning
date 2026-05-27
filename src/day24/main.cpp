#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <memory>
#include <new>

LOG_MODULE_REGISTER(day24_memory, LOG_LEVEL_INF);

// Zephyr bounded heap (deterministic max memory usage)
K_HEAP_DEFINE(app_heap, 1024);  // 1KB bounded heap

// Object pool using placement new
template <typename T, size_t N>
class ObjectPool {
public:
    T *acquire() {
        for (size_t i = 0; i < N; i++) {
            if (!used_[i]) {
                used_[i] = true;
                return new (&storage_[i]) T{};  // Placement new — no heap!
            }
        }
        return nullptr;  // Pool exhausted
    }
    void release(T *obj) {
        for (size_t i = 0; i < N; i++) {
            if (reinterpret_cast<T *>(&storage_[i]) == obj) {
                obj->~T();      // Explicit destructor call required with placement new
                used_[i] = false;
                return;
            }
        }
    }
private:
    alignas(T) uint8_t storage_[N][sizeof(T)];
    bool used_[N] = {};
};

struct Message {
    uint8_t  cmd;
    uint8_t  data[16];
    uint16_t len;
    Message() : cmd(0), len(0) { memset(data, 0, sizeof(data)); }
};

static ObjectPool<Message, 4> msg_pool;

// unique_ptr: zero-overhead RAII, moves ownership
static void unique_ptr_demo(void)
{
    // Allocate from Zephyr heap with unique_ptr
    auto *raw = static_cast<uint8_t *>(k_heap_alloc(&app_heap, 64, K_NO_WAIT));
    if (!raw) { LOG_ERR("Heap alloc failed"); return; }

    // Manual RAII with unique_ptr and custom deleter
    auto deleter = [](uint8_t *p) {
        k_heap_free(&app_heap, p);
        LOG_INF("Heap block freed by unique_ptr deleter");
    };
    std::unique_ptr<uint8_t, decltype(deleter)> buf(raw, deleter);

    buf[0] = 0xDE;
    buf[1] = 0xAD;
    LOG_INF("Heap buf[0..1]: 0x%02X 0x%02X", buf[0], buf[1]);
    // Freed automatically when buf goes out of scope
}

int main(void)
{
    // Object pool usage (no heap)
    Message *m1 = msg_pool.acquire();
    Message *m2 = msg_pool.acquire();
    if (m1 && m2) {
        m1->cmd = 0x01; m1->len = 4;
        m2->cmd = 0x02; m2->len = 8;
        LOG_INF("Pool acquired 2 messages");
        LOG_INF("m1: cmd=0x%02X len=%u", m1->cmd, m1->len);
        LOG_INF("m2: cmd=0x%02X len=%u", m2->cmd, m2->len);
        msg_pool.release(m1);
        msg_pool.release(m2);
        LOG_INF("Pool released 2 messages");
    }

    // Re-acquire after release
    Message *m3 = msg_pool.acquire();
    LOG_INF("Re-acquired: %s", m3 ? "success" : "failed");
    if (m3) msg_pool.release(m3);

    // unique_ptr with Zephyr heap
    unique_ptr_demo();

    while (1) { k_msleep(5000); }
    return 0;
}
