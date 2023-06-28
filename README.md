# LEVPOS

This is a remake of the firmware done in Nano\_TinyLev for an acustic levitator
using a STM32 microcontroller and providing a control interface.

Features:
- The wave generation was simplified by using a timer peripheral
- A serial protocol was implemented to allow software control
- A python script was included to track a target and command the board to move
to the desired position

Pins:
- Channel 1 : PA8 and PA7, in nucleo board: D7 and D11
- Channel 2 : PA9 and PB0, in nucleo board: D8 and A3
- UART: PA2 (tx) and PA3 (rx), in nucleo board goes to UART-USB adapter
