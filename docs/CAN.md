# CAN
CAN Bus is a network of two wires, intertwined together, that allow simple and rugged wiring. 

## Scope
This document explains more about CAN and how we will utilize it in our systems to communicate between different hareware modules.

## Message Components
CAN Message is made of multiple parts

|Component|Bits|
|-|-|
|SOF|1|
|Atrribution Field|12|
|Control Field|6|
|Data|64|
|CRC|15|
|Delimiter|1|
|ACK|1|
|Delimiter|1|
|EOF|7|
|IFS|3|

### Start of Frame (SOF)
*SOF* Marks the beginning of data the frame. Each SOF is 1 bit long

### Attribution Field
Contains the message identifier and the RTR bit. The identifier is a unique value that determines message priority, with lower values indicating higher priority. 

#### Message ID
The *ID* makes the message unique but also determines the priority of the message. Each ID is 11 bits long.

**LOWER** number IDs have a higher priority to **HIGHER** IDs.  

Because the CAN Bus is only two wires running throughout the entire car, the messages have to be prioritized. If not, important messages may be delayed due to a busy wire. The message ID is a list of 1's and 0's that create an identifier (like a badge of authority) that should what is of highest priority. 0 represents a dominant identification while a 1 is recesive. If two messages want to be sent at the same time, they will both send zeros until one of the identifiers represents a one. It is at that point that the message with a one "gives up" and the other message is sent through the CAN Bus.

##### Example
Take the following message IDs and lets compare which would have a higher priority.
```
ID: 0x100 → binary 0001 0000 0000
ID: 0x080 → binary 0000 1000 0000
ID: 0x120 → binary 0001 0010 0000
```
In this example, the `0x080` or `0000 1000 0000` will always win when attempting to enter the CAN Bus

#### Remote Transmission Request (RTR)
a bit within a CAN message that distinguishes between a Data Frame and a Remote Frame

### Control Field
Contains several bits, including the Identifier Extension bit (IDE), which distinguishes between standard and extended frames, and the Data Length Code (DLC), which specifies the number of bytes in the data field. 

### Data Field
Carries the actual message payload. It can contain 0 to 8 bytes (64 bits) of data. 

### Cycle Redundancy Check (CRC) Field
Contains a Cyclic Redundancy Check value for error detection. It includes a CRC sequence and a CRC delimiter. 

### Acknowledgement (ACK) Field
The acknowledgment slot where a receiving node sends a dominant bit to confirm successful reception of the message. This field is followed by an ACK delimiter. 

### End of Frame (EOF)
Consists of seven recessive bits that signify the end of the message frame. 

### Interframe Space (IFS)
A period of bus inactivity that occurs after the End of Frame, separating one frame from the next. 

## Resources
<!-- Links to additional or external resources -->