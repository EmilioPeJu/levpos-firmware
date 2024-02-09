#include <stdbool.h>
#include <stdint.h>

#include "peripherals.h"
#include "phase_control.h"

static int16_t step = 0;
static const uint16_t SOFT_PSC = 4;

void phase_control_set_speed(int16_t val)
{
    step = val;
}

static void _step_ch_1(int16_t step_1)
{
    static bool inverted = false;
    static int16_t count_ch_1 = 0;
    bool polarity_changed = false;
    count_ch_1 += step_1;

    if (count_ch_1 >= PHASE_N)
        count_ch_1 -= PHASE_N;
    else if (count_ch_1 < PHASE_LOW)
        count_ch_1 += PHASE_N;

    if (count_ch_1 >= PHASE_MID) {
        WAVE_TIMER.Instance->CCR1 = count_ch_1 - PHASE_MID;
        if (!inverted) {
            inverted = true;
            polarity_changed = true;
            WAVE_TIMER.Instance->CCER |= TIM_CCER_CC1P | TIM_CCER_CC1NP;
        }
    } else if (count_ch_1 < PHASE_MID) {
        WAVE_TIMER.Instance->CCR1 = count_ch_1;
        if (inverted) {
            inverted = false;
            polarity_changed = true;
            WAVE_TIMER.Instance->CCER &= ~(TIM_CCER_CC1P | TIM_CCER_CC1NP);
        }
    }

    if (polarity_changed)
        WAVE_TIMER.Instance->EGR |= TIM_EGR_UG;
}

static void _step_ch_2(int16_t step_2)
{
    static bool inverted = false;
    static int16_t count_ch_2 = 0;
    bool polarity_changed = false;
    count_ch_2 += step_2;

    if (count_ch_2 >= PHASE_N)
        count_ch_2 -= PHASE_N;
    else if (count_ch_2 < PHASE_LOW)
        count_ch_2 += PHASE_N;

    if (count_ch_2 >= PHASE_MID) {
        WAVE_TIMER.Instance->CCR2 = count_ch_2 - PHASE_MID;
        if (!inverted) {
            inverted = true;
            polarity_changed = true;
            WAVE_TIMER.Instance->CCER |= TIM_CCER_CC2P | TIM_CCER_CC2NP;
        }
    } else if (count_ch_2 < PHASE_MID) {
        WAVE_TIMER.Instance->CCR2 = count_ch_2;
        if (inverted) {
            inverted = false;
            polarity_changed = true;
            WAVE_TIMER.Instance->CCER &= ~(TIM_CCER_CC2P | TIM_CCER_CC2NP);
        }
    }

    if (polarity_changed)
        WAVE_TIMER.Instance->EGR |= TIM_EGR_UG;
}

void phase_control_step()
{
    static uint16_t soft_psc_count = 0;
    if (++soft_psc_count < SOFT_PSC)
        return;

    soft_psc_count = 0;
    if (step < 0)
        _step_ch_2(step);
    else if (step > 0)
        _step_ch_1(-step);
}
