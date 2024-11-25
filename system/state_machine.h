#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "pico/stdlib.h"
#include "command_sequence.h"

void state_machine(at_command_state_t *state, absolute_time_t *command_time, absolute_time_t *led_time, int *at_command_counter);

#endif // STATE_MACHINE_H
