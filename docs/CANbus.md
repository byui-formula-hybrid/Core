# CAN

## Definitions
- **Word** - Definition of the word

## Overview
CAN Bus is a network of two wires, intertwined together, that allow simple and rugged wiring. 

#### Parts of a CAN Message
 1. **ID** Arbitration is the identifier of the message. Because the CAN Bus is only two wires running throughout the entire car, the messages have to be prioritized. If not, important messages may be delayed due to a busy wire. The message ID is a list of 1's and 0's that create an identifier (like a badge of authority) that should what is of highest priority. 0 represents a dominant identification while a 1 is recesive. If two messages want to be sent at the same time, they will both send zeros until one of the identifiers represents a one. It is at that point that the message with a one "gives up" and the other message is sent through the CAN Bus. 
 
    For example:
        
        ID: 0x100 → binary 0001 0000 000
        ID: 0x080 → binary 0000 1000 0000
        ID: 0x120 → binary 0001 0010 0000
    In this example, the 0x080 or 0000 1000 0000 will always win when attempting to enter the CAN Bus

    
    
    
    
2. Control Field

- Contains information about the message structure, such as:

- Data Length Code (DLC): how many data bytes are in the message (0–8 bytes for CAN 2.0, up to 64 for CAN FD).

- Flags that indicate if the message uses standard or extended ID.
    Example CAN message map

    CAN Message Map Example: 

    **Arbitration ID:** `0x201`  
    **Data Length Code (DLC):** 8 bytes  

    | Byte # | Bits     | Signal Name         | Meaning                            | Example Value   |
    |--------|----------|---------------------|------------------------------------|-----------------|
    | Byte 0 | Bit 0    | Engine ON/OFF       | 0 = Off, 1 = On                    | 1 (engine on)   |
    |        | Bit 1    | Check Engine Light  | 0 = Off, 1 = On                    | 0 (off)         |
    |        | Bits 2–7 | Reserved            | Unused                             | 0               |
    | Byte 1–2 | All 16 | Engine RPM          | (Value ÷ 4) = RPM                  | 0x0FA0 → 4000   |
    | Byte 3 | All 8    | Coolant Temperature | Value – 40 = °C                    | 0x64 → 100 °C   |
    | Byte 4 | All 8    | Throttle Position   | Percent (0–255 = 0–100%)           | 0x80 → 50%      |
    | Byte 5 | All 8    | Vehicle Speed       | Value in km/h                      | 0x3C → 60 km/h  |
    | Byte 6 | All 8    | Fuel Level          | Percent (0–255 = 0–100%)           | 0xC8 → 80%      |
    | Byte 7 | All 8    | Reserved            | Not used (set to 0)                | 0x00            |

    ---
    ID: 0x201
    Data: [ 01 | 0F | A0 | 64 | 80 | 3C | C8 | 00 ]
    **Decoded:**
    - Engine ON  
    - RPM = 4000  
    - Coolant Temperature = 100 °C  
    - Throttle = 50%  
    - Speed = 60 km/h  
    - Fuel = 80%  

    Byte #: 0 1 2 3 4 5 6 7
    +--------+--------+--------+--------+--------+--------+--------+--------+
    Bits: |00000001|00001111|10100000|01100100|10000000|00111100|11001000|00000000|+--------+--------+--------+--------+--------+--------+--------+--------+

    Meaning: |Eng/Chk | Engine RPM | Coolant |Throttle | Speed | Fuel |
    | Flags | | Temp | % | km/h | % |


### Example Data

 

#### Limitation of CAN Message

#### Sending Multiple Parts of data in one Message

#### How do I identifying one message from another?

#### Sending large numbers

#### Sending Small numbers

#### High & Low Can

#### Sending Strings

## Resources
[Text will be seen](https://google.com)