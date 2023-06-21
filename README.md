# MTT Board Firmware
Firmware for the UCT Mechatronics Teaching Tool custom board

Written/compiled with STM32CubeIDE using the HAL library: https://www.st.com/en/development-tools/stm32cubeide.html

## Getting Started
1. Clone this repo
### Flashing the board
2. Open the `MTT-Firmware` folder in STM32CubeIDE
3. Connect to the debugger port with the ST-Link attached
### Connecting the board to the PC
4. Remove the ST-Link debugger
5. Plug the micro USB into the non-debugger port on the board
### Installing the MTT Simulink Library
6. Move the `MTT Simulink Library` folder onto the MATLAB path
   - Windows — %USERPROFILE%/Documents/MATLAB
   - MacOS/Linux - $home/Documents/MATLAB
9. Refresh the Library Browser with `refresh(lb)` in the MATLAB Command Window
### Running the example
10. Open `example.slx`
11. Double click the `MTT Basic I/O` block and select the correct serial port. The board should be plugged in before opening Simulink.
12. Run the model and adjust the sliders, buttons, toggles, and potentiometers and ensure everything is responding.

## Serial Communication Protocol
Communication is structured in a request-response format with a rate of 115200 baud. A request is a message from the PC to the MTT and a response is a message from the MTT back to the PC. The packet header determines the type of request-response. This communication protocol was chosen as it allows the PC to set the communication rate and other request-reponse messages can be added without breaking existing ones (Important if multiple courses will be using it).

Currently only one request-response has been added with the header 0xAA for the MEC3074F course, but others can be added and should be documented here.
| Header | Course   | Message Description |
|--------|----------|---------------------|
| 0xAA   | MEC3074F | This message was constructed to replace the functionality of the USB-6009 DAQ. Request contains digital outputs for LED's and segment display and responds with potentiometer readings, and button & switch state. |

### 0xAA (MEC3074) Message Packet Format

#### Request
| Field    | Size (bytes) | Description                               |
|----------|--------------|-------------------------------------------|
| Header   | 1            | Packet header (0xAA)                      |
| LED's    | 1            | 8x binary LED states          |
| Segment Display | 2     | 11x binary segment display states         |

#### Response
| Field    | Size (bytes) | Description                               |
|----------|--------------|-------------------------------------------|
| Header   | 1            | Packet header (0xAA)                      |
| Potentiometers    | 8   | 4x unit16 potentiometer readings          |
| Buttons & Toggles | 1   | 4x binary button states and 4x binary toggle states |

## I/O
**Implemented**
- LED's
- Buttons
- Toggle switches
- Segment Display
- Potentiometers

## Simulink Blocks
The idea was for there to be a MTT Library which students can access from the Library Browser which contains blocks for basic I/O, using the IMU, servo control, timers, etc. Each of these blocks will correspond to a specific request-response interaction with a unique header. The file `mttlib.slx` is the library file for the blocks and `slblocks.m` ensures it displays correctly in the Library Browser. Currently only the Basic I/O block has been implemented but this can be used as an example for future blocks (See the `example.slx` file for how to use this block). A masked Simulink block was made to handle the Basic I/O message. A mask is important to allow easy configuration of the important block sample time and serial port parameters without overwhelming students with unnecessary complexity hidden inside the block.

The only issue with this MTT Library approach is that adding the folder to the MATLAB path requires a few steps and it will need to be done each time the students login to do a prac. An alternative is that a single file with all of the necessary blocks is provided at the start of the prac, and students can just copy blocks as they need them.

## Noted Hardware Issues
v1 - Cannot use serial with ST-Link debugger connected.
