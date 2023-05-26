#ifndef _SERIAL_H
#define _SERIAL_H

#include <stdbool.h>

#include "ring_buffer.h"

extern struct ring_buffer host_serial_buffer;

#define MAX_COMMAND_SIZE 256
#define TERMINATOR 10
#define TX_SERIAL_TIMEOUT 16  // ms
#define RX_SERIAL_TIMEOUT 16  // ms

bool serial_init();

void serial_handle_rx();

#endif
