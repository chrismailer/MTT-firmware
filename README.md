# MTT-firmware
Firmware for the UCT Mechatronics Teaching Tool

Written/compiled with STM32CubeIDE: https://www.st.com/en/development-tools/stm32cubeide.html

## Communication Protocol
Communication is structured in a ping-pong interaction. The PC send a message to the MTT with some data (ping) and based on this message will get a specific response (pong).


## Hardware Issues
v1 - Cannot use serial with ST-Link debugger connected.