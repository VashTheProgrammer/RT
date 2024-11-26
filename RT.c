#include <stdio.h>
#include "hardware/uart.h"
#include "pico/stdlib.h"

#include "command_sequence.h"
#include "state_machine.h"
#include "config.h"
#include "scheduler.h"

#define UART0_TX_PIN 0
#define UART0_RX_PIN 1
#define UART1_TX_PIN 4
#define UART1_RX_PIN 5
#define LED_PIN 25

#define BAUD_RATE 115200

// CLOCK_PER_SEC in RP2040 è 100 !

bool led_on = false;

void task_1(void){
    led_on = !led_on;
    gpio_put(LED_PIN, led_on);
}

void task_2(void){
    // Se c'è un dato disponibile sulla UART0 (PC), invialo alla UART1 (ESP32)
    if (uart_is_readable(uart0)) {
        uint8_t data = uart_getc(uart0);
        uart_putc(uart1, data);
    }

    // Se c'è un dato disponibile sulla UART1 (ESP32), invialo alla UART0 (PC)
    if (uart_is_readable(uart1)) {
        uint8_t data = uart_getc(uart1);
        uart_putc(uart0, data);
    }
}

at_command_state_t state = STATE_SEND_AT_GMR;  // Stato iniziale
int at_command_counter = 0;

void task_3(void){
    state_machine(&state, &at_command_counter);
}

int main()
{
    // Inizializza la configurazione della UART
    stdio_init_all();
    
    // Inizializza UART0 (connessione al PC)
    uart_init(uart0, BAUD_RATE);
    gpio_set_function(UART0_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART0_RX_PIN, GPIO_FUNC_UART);
    
    // Inizializza UART1 (connessione all'ESP32)
    uart_init(uart1, BAUD_RATE);
    gpio_set_function(UART1_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART1_RX_PIN, GPIO_FUNC_UART);

    // Inizializza il pin del LED
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);    

    // Aggiorna i comandi con i parametri configurati
    update_command_sequence(&iot_config);
    at_command_state_t state = STATE_WAIT_BEFORE_START;

    // Aggiungi alcuni task allo scheduler
    scheduler_add_task(task_1, 2, 50);     // Priorità 2, intervallo 50 ticks -> 500 millisec
    scheduler_add_task(task_2, 1, 0.1);     // Priorità 1, intervallo 0.1 ticks -> 1 millisec
    scheduler_add_task(task_3, 3, 500);     // Priorità 1, intervallo 500 ticks -> 5000 millisec

    // Avvia lo scheduler (loop infinito)
    scheduler_run();

    return 0;
}
