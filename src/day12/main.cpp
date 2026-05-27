#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <array>

LOG_MODULE_REGISTER(day12_arrays, LOG_LEVEL_INF);

// Static ring buffer (manual implementation preview of Day 26)
template <typename T, size_t N>
class StaticRingBuffer {
public:
    bool push(const T &item) {
        if (full()) return false;
        buf_[head_] = item;
        head_ = (head_ + 1) % N;
        count_++;
        return true;
    }
    bool pop(T &out) {
        if (empty()) return false;
        out = buf_[tail_];
        tail_ = (tail_ + 1) % N;
        count_--;
        return true;
    }
    bool empty() const { return count_ == 0; }
    bool full()  const { return count_ == N; }
    size_t size() const { return count_; }
private:
    std::array<T, N> buf_{};
    size_t head_  = 0;
    size_t tail_  = 0;
    size_t count_ = 0;
};

// Zephyr memory slab: fixed-size block allocator (deterministic, no fragmentation)
K_MEM_SLAB_DEFINE(my_slab, 32, 4, 4);  // 4 blocks of 32 bytes, 4-byte aligned

int main(void)
{
    // std::array: stack-allocated, no heap, has .size() and bounds checks
    std::array<uint8_t, 16> uart_buf{};
    uart_buf.fill(0xFF);
    uart_buf[0] = 0xAA;
    LOG_INF("uart_buf[0]=0x%02X size=%zu", uart_buf[0], uart_buf.size());

    // Static ring buffer
    StaticRingBuffer<uint16_t, 8> adc_fifo;
    for (uint16_t i = 0; i < 5; i++) {
        adc_fifo.push(i * 100);
    }
    LOG_INF("FIFO size: %zu", adc_fifo.size());
    uint16_t sample;
    while (adc_fifo.pop(sample)) {
        LOG_INF("  sample: %u", sample);
    }

    // Zephyr mem_slab usage
    void *block = nullptr;
    if (k_mem_slab_alloc(&my_slab, &block, K_NO_WAIT) == 0) {
        memset(block, 0xBB, 32);
        LOG_INF("Slab block allocated at: 0x%08X", (uint32_t)(uintptr_t)block);
        k_mem_slab_free(&my_slab, block);
        LOG_INF("Slab block freed");
    }
    LOG_INF("Free slab blocks: %u", k_mem_slab_num_free_get(&my_slab));

    while (1) { k_msleep(5000); }
    return 0;
}
