#include <stdio.h>

#include "command_sequence.h"
#include "config.h"


at_command_t at_command_sequence[] = {
    {STATE_SEND_AT_GMR, "", "Inviando AT+GMR alla UART1", STATE_SEND_AT_CWMODE},
    {STATE_SEND_AT_CWMODE, "", "Inviando AT+CWMODE=1 alla UART1", STATE_SEND_AT_CONNECT_WIFI},
    {STATE_SEND_AT_CONNECT_WIFI, "", "Inviando AT+CWJAP alla UART1", STATE_SEND_AT_CWJAP},
    {STATE_SEND_AT_CWJAP, "", "Inviando AT+CWJAP? alla UART1", STATE_SEND_AT_CIFSR},
    {STATE_SEND_AT_CIFSR, "", "Inviando AT+CIFSR alla UART1", STATE_COMPLETE},
    {STATE_COMPLETE, "", "Configurazione completata.", STATE_IDLE}
};

bool update_command_sequence(iot_config_t *config) 
{
    if (config == NULL) {
        printf("Error: config is NULL\n");
        return false;
    }

    int result = 0;

    result = snprintf(at_command_sequence[0].command, sizeof(at_command_sequence[0].command), "AT+GMR\r\n");
    if (result < 0 || result >= (int)sizeof(at_command_sequence[0].command)) {
        printf("Error: command 0 truncated or failed\n");
        return false;
    }

    result = snprintf(at_command_sequence[1].command, sizeof(at_command_sequence[1].command), "AT+CWMODE=1\r\n");
    if (result < 0 || result >= (int)sizeof(at_command_sequence[1].command)) {
        printf("Error: command 1 truncated or failed\n");
        return false;
    }

    result = snprintf(at_command_sequence[2].command, sizeof(at_command_sequence[2].command), "AT+CWJAP=\"%s\",\"%s\"\r\n", config->wifi_ssid, config->wifi_password);
    if (result < 0 || result >= (int)sizeof(at_command_sequence[2].command)) {
        printf("Error: command 2 truncated or failed\n");
        return false;
    }

    result = snprintf(at_command_sequence[3].command, sizeof(at_command_sequence[3].command), "AT+CWJAP?\r\n");
    if (result < 0 || result >= (int)sizeof(at_command_sequence[3].command)) {
        printf("Error: command 3 truncated or failed\n");
        return false;
    }

    result = snprintf(at_command_sequence[4].command, sizeof(at_command_sequence[4].command), "AT+CIFSR\r\n");
    if (result < 0 || result >= (int)sizeof(at_command_sequence[4].command)) {
        printf("Error: command 3 truncated or failed\n");
        return false;
    }

    return true;
}


