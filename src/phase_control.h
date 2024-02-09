#ifndef _PHASE_CONTROL_H
#define _PHASE_CONTROL_H

#include <stdint.h>

#define PHASE_N 400
#define PHASE_LOW 0
#define PHASE_MID 200
#define PHASE_HIGH (PHASE_N - 1)

void phase_control_set_speed(int16_t val);

void phase_control_step();

#endif
