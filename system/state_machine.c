#include "state_machine.h"
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "command_sequence.h"
#include <stdio.h>

void state_machine(at_command_state_t *state, int *at_command_counter) {
    for (int i = 0; at_command_sequence[i].state != STATE_IDLE; i++) {
        if (at_command_sequence[i].state == *state) {
            if (uart_is_writable(uart1)) {
                if (at_command_sequence[i].command[0] != '\0') {
                    (*at_command_counter)++;
                    // Stampa su UART0 che stai inviando il comando e il numero di richieste
                    printf("%s, richiesta numero: %d\n", at_command_sequence[i].message, *at_command_counter);
                    uart_puts(uart1, at_command_sequence[i].command);
                } else {
                    printf("%s\n", at_command_sequence[i].message);
                }
                *state = at_command_sequence[i].next_state;
            }
            break;
        }
    }
}
