#include "state_machine.h"
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "command_sequence.h"
#include <stdio.h>

#define COMMAND_INTERVAL_MS 10000  // Intervallo di tempo tra i comandi AT

void state_machine(at_command_state_t *state, absolute_time_t *command_time, absolute_time_t *led_time, int *at_command_counter) {
    switch (*state) {
        case STATE_WAIT_BEFORE_START:
            // Attendi 2 secondi prima di iniziare
            if (absolute_time_diff_us(get_absolute_time(), *command_time) <= 0) {
                *state = STATE_SEND_AT_RST;
                *command_time = make_timeout_time_ms(2000);  // Attesa di 2 secondi dopo il reset
            }
            break;

        case STATE_SEND_AT_RST:
            if (uart_is_writable(uart1)) {
                printf("Inviando AT+RST alla UART1\n");
                uart_puts(uart1, "AT+RST\r\n");
                *state = STATE_WAIT_AFTER_RST;
            }
            break;

        case STATE_WAIT_AFTER_RST:
            // Attendi 5 secondi dopo il comando di reset
            if (absolute_time_diff_us(get_absolute_time(), *command_time) <= 0) {
                *state = STATE_SEND_AT_GMR;
                *command_time = make_timeout_time_ms(COMMAND_INTERVAL_MS);
            }
            break;

        default:
            for (int i = 0; at_command_sequence[i].state != STATE_IDLE; i++) {
                if (at_command_sequence[i].state == *state) {
                    // Attendi fino a quando non è il momento di inviare il prossimo comando
                    if (absolute_time_diff_us(get_absolute_time(), *command_time) <= 0) {
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
                            *command_time = make_timeout_time_ms(COMMAND_INTERVAL_MS);
                        }
                    }
                    break;
                }
            }
            break;

        case STATE_IDLE:
            // Macchina a stati ferma, non fa nulla
            break;
    }
}
