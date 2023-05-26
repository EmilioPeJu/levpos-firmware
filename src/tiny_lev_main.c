#include <stdio.h>

#include "tiny_lev_main.h"
#include "main.h"
#include "cli.h"
#include "serial.h"

void tiny_lev_main()
{
    serial_init();
    while(1) {
        cli_handle();
    }
}
