#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

#include "commands.h"
#include "peripherals.h"
#include "phase_control.h"
#include "serial.h"
#include "util.h"

static bool help_command(char *args)
{
    printf("+The following commands are supported:\r\n");
    struct command_description *command;
    size_t command_index;
    for_each_command (command, command_index) {
        printf("+ %16s -> %s\r\n", command->name, command->help);
    }
    printf(".\n");

    return true;
}

static bool ping_command(char *args)
{
    printf("OK pong\r\n");
    return true;
}

static bool reset_command(char *args)
{
    printf("OK Resetting the MCU\r\n");
    // give the UART chance to send out the message
    HAL_Delay(50);
    HAL_NVIC_SystemReset();
    return true;
}

static bool set_increment_command(char *args)
{
        int16_t inc = 0;
        int nargs = sscanf(args, "%" PRIi16, &inc);
        if (nargs != 1) {
                printf("ERR Invalid format\r\n");
                return false;
        }

        phase_control_set_increment(inc);
        printf("OK\r\n");

        return true;
}

static bool set_phase_command(char *args)
{
        int16_t phase = 0;
        int nargs = sscanf(args, "%" PRIi16, &phase);
        if (nargs != 1) {
                printf("ERR Invalid format\r\n");
                return false;
        }

        phase_control_set_phase(phase);
        printf("OK\r\n");

        return true;
}

static bool set_speed_command(char *args)
{
        int16_t speed = 0;
        int nargs = sscanf(args, "%" PRIi16, &speed);
        if (nargs != 1) {
                printf("ERR Invalid format\r\n");
                return false;
        }

        phase_control_set_speed(speed);
        printf("OK\r\n");

        return true;
}

struct command_description command_table[] = {
    {"help", help_command, "Generates a command list"},
    {"i", set_increment_command, "Set increment in phase"},
    {"p", set_phase_command, "Set phase"},
    {"ping", ping_command, "Return a pong"},
    {"reset", reset_command, "Reset MCU"},
    {"s", set_speed_command, "Set speed"}
};

size_t command_table_size = ARRAY_SIZE(command_table);

void execute_command_line(char *line)
{
    struct command_description *command_entry;
    size_t command_index;
    char *cmd_end = strchr(line, ' ') ?:
                    strchr(line, 13) ?:
                    strchr(line, 10) ?:
                    line + strlen(line);
    size_t line_command_len = cmd_end - line;
    for_each_command (command_entry, command_index) {
        size_t command_len = strlen(command_entry->name);
        if (line_command_len == command_len
                && strncmp(line, command_entry->name, command_len) == 0) {
            while (*cmd_end == ' ' && *cmd_end != 0)
                cmd_end++;
            command_entry->function(cmd_end);
            return;
        }
    }
    printf("ERR Command not found: %s\r\n", line);
}
