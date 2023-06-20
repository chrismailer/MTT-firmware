# MTT Board Firmware
Firmware for the UCT Mechatronics Teaching Tool custom board

Written/compiled with STM32CubeIDE using the HAL library: https://www.st.com/en/development-tools/stm32cubeide.html

## Serial Communication Protocol
Communication is structured in a request-response format. A request is a message from the PC to the MTT and a response is a message from the MTT back to the PC. The packet header determines the type of request-response. This communication protocol was chosen as it allows the PC to set the communication rate and other request-reponse messages can be added without breaking existing ones (Important if multiple courses will be using it).

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
| Potentiometers | 8      | 4x unit16 potentiometer readings          |
| Buttons & Toggles | 1     | 4x binary button states and 4x binary toggle states |

## I/O
**Implemented**
- LED's
- Buttons
- Toggle switches
- Segment Display
- Potentiometers

## Noted Hardware Issues
v1 - Cannot use serial with ST-Link debugger connected.
