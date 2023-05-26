#include <stdbool.h>

#include "commands.h"
#include "serial.h"

#include "cli.h"

void cli_handle()
{
    static uint8_t _cli_cmd[MAX_COMMAND_SIZE + 1];
    static uint16_t _cli_cmd_i = 0;
    uint8_t rx_char = 0;
    while (!rb_is_empty(&host_serial_buffer) &&
            rx_char != TERMINATOR &&
            _cli_cmd_i < MAX_COMMAND_SIZE) {
        rx_char = rb_get(&host_serial_buffer);
        _cli_cmd[_cli_cmd_i++] = rx_char;
    }
    _cli_cmd[_cli_cmd_i] = 0;
    if (rx_char == 10 || rx_char == 13 || _cli_cmd_i >= MAX_COMMAND_SIZE) {
        execute_command_line((char *) _cli_cmd);
        _cli_cmd_i = 0;
    }
}
