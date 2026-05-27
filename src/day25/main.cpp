#include <zephyr/kernel.h>
#include <zephyr/smf.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(day25_fsm, LOG_LEVEL_INF);

// ── Simple enum class FSM ─────────────────────────────────────────────────
enum class LedFsmState : uint8_t { IDLE, BLINKING, ALARM, OFF };
enum class LedFsmEvent : uint8_t { START, STOP, ALARM_ON, RESET };

class LedFsm {
public:
    LedFsm() : state_(LedFsmState::IDLE) {}

    void dispatch(LedFsmEvent event) {
        LedFsmState next = state_;
        switch (state_) {
        case LedFsmState::IDLE:
            if (event == LedFsmEvent::START) next = LedFsmState::BLINKING;
            break;
        case LedFsmState::BLINKING:
            if (event == LedFsmEvent::STOP)     next = LedFsmState::IDLE;
            if (event == LedFsmEvent::ALARM_ON) next = LedFsmState::ALARM;
            break;
        case LedFsmState::ALARM:
            if (event == LedFsmEvent::RESET)    next = LedFsmState::IDLE;
            break;
        default: break;
        }
        if (next != state_) {
            on_exit(state_);
            state_ = next;
            on_entry(state_);
        }
    }
    LedFsmState state() const { return state_; }
private:
    void on_entry(LedFsmState s) {
        const char *names[] = {"IDLE","BLINKING","ALARM","OFF"};
        LOG_INF("FSM -> %s", names[(int)s]);
    }
    void on_exit(LedFsmState s) { (void)s; }
    LedFsmState state_;
};

// ── Zephyr SMF example ────────────────────────────────────────────────────
struct app_state {
    struct smf_ctx ctx;     // MUST be first member
    uint32_t blink_count;
};

static void state_idle_entry(void *o)   { LOG_INF("SMF: IDLE entry"); }
static void state_idle_run(void *o)     { LOG_INF("SMF: IDLE run"); }
static void state_active_entry(void *o) { LOG_INF("SMF: ACTIVE entry"); }
static void state_active_run(void *o) {
    struct app_state *s = (struct app_state *)o;
    s->blink_count++;
    LOG_INF("SMF: ACTIVE blink #%u", s->blink_count);
}

static const struct smf_state app_states[] = {
    [0] = SMF_CREATE_STATE(state_idle_entry,   state_idle_run,   NULL, NULL, NULL),
    [1] = SMF_CREATE_STATE(state_active_entry, state_active_run, NULL, NULL, NULL),
};

int main(void)
{
    // Simple FSM
    LedFsm fsm;
    fsm.dispatch(LedFsmEvent::START);
    fsm.dispatch(LedFsmEvent::ALARM_ON);
    fsm.dispatch(LedFsmEvent::RESET);
    fsm.dispatch(LedFsmEvent::START);
    fsm.dispatch(LedFsmEvent::STOP);

    // Zephyr SMF
    struct app_state s = {};
    smf_set_initial((struct smf_ctx *)&s, &app_states[0]);

    for (int i = 0; i < 3; i++) {
        smf_run_state((struct smf_ctx *)&s);
        k_msleep(200);
    }

    // Transition to active
    smf_set_state((struct smf_ctx *)&s, &app_states[1]);
    for (int i = 0; i < 3; i++) {
        smf_run_state((struct smf_ctx *)&s);
        k_msleep(200);
    }

    while (1) { k_msleep(5000); }
    return 0;
}
