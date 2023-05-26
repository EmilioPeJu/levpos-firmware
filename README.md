# LEVPOS

This is a remake of the firmware done in Nano\_TinyLev for an acustic levitator
using a STM32 microcontroller and providing a control interface.

Features:
- The wave generation was simplified by using a timer peripheral
- A serial protocol was implemented to allow software control
- A python script was included to track a target and command the board to move
to the desired position
