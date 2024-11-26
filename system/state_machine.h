#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "pico/stdlib.h"
#include "command_sequence.h"

void state_machine(at_command_state_t *state, int *at_command_counter);

#endif // STATE_MACHINE_H
