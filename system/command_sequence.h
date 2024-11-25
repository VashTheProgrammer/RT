#ifndef COMMAND_SEQUENCE_H
#define COMMAND_SEQUENCE_H

#include "config.h"

typedef enum {
    STATE_WAIT_BEFORE_START,
    STATE_SEND_AT_RST,
    STATE_WAIT_AFTER_RST,
    STATE_SEND_AT_GMR,
    STATE_SEND_AT_CWMODE,
    STATE_SEND_AT_CONNECT_WIFI,
    STATE_SEND_AT_CWJAP,
    STATE_SEND_AT_CIFSR,
    STATE_COMPLETE,
    STATE_IDLE
} at_command_state_t;

typedef struct {
    at_command_state_t state;
    char command[256];
    const char *message;
    at_command_state_t next_state;
} at_command_t;

extern at_command_t at_command_sequence[];
bool update_command_sequence(iot_config_t *config);

#endif // COMMAND_SEQUENCE_H
