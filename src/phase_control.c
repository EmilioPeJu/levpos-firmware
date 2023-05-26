#include <stdbool.h>
#include <stdint.h>

#include "peripherals.h"
#include "phase_control.h"


static int16_t step = 0;
static int16_t increment = 0;
static int16_t count = 0;

void phase_control_set_speed(int16_t val)
{
    step = val;
}

void phase_control_set_increment(int16_t val)
{
    increment = val;
}

void phase_control_set_phase(int16_t val)
{
    count = val;
}

void phase_control_step()
{
    static bool inverted = false;
    count += step + increment;
    increment = 0;

    count = count % PHASE_N;
    if (count < PHASE_LOW)
        count += PHASE_N;

    if (count >= PHASE_MID) {
        count -= PHASE_MID;
        WAVE_TIMER.Instance->CCR2 = count;
        if (!inverted) {
            inverted = true;
            WAVE_TIMER.Instance->CCER |= TIM_CCER_CC2P | TIM_CCER_CC2NP;
            WAVE_TIMER.Instance->EGR |= TIM_EGR_UG;
        }
    } else if (count < PHASE_MID && inverted) {
        inverted = false;
        WAVE_TIMER.Instance->CCR2 = count;
        WAVE_TIMER.Instance->CCER &= ~(TIM_CCER_CC2P | TIM_CCER_CC2NP);
        WAVE_TIMER.Instance->EGR |= TIM_EGR_UG;
    } else {
        WAVE_TIMER.Instance->CCR2 = count;
    }
}
