#include <zephyr/kernel.h>

static constexpr uint16_t pwm_table[8] = {0, 36, 73, 109, 146, 182, 218, 255};

enum class BattState : uint8_t { FULL, OK, LOW, CRITICAL };

static BattState classify(uint16_t mv) {
    if (mv > 4000) return BattState::FULL;
    if (mv > 3500) return BattState::OK;
    if (mv > 3200) return BattState::LOW;
    return BattState::CRITICAL;
}

static const char *batt_str(BattState s) {
    switch (s) {
    case BattState::FULL:     return "FULL";
    case BattState::OK:       return "OK";
    case BattState::LOW:      return "LOW";
    case BattState::CRITICAL: return "CRITICAL";
    default:                  return "?";
    }
}

int main(void)
{
    uint16_t mv = 3700;
    printk("Battery %u mV: %s\n", mv, batt_str(classify(mv)));
    for (uint8_t i = 0; i < 8; i++) {
        printk("PWM[%u] = %u\n", i, pwm_table[i]);
    }
    while (1) { k_msleep(5000); }
    return 0;
}
