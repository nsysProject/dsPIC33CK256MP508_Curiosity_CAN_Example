<h1>Description</h1>
dsPIC33CK256MP508_Curiosity_CAN_Example describes the hardware connection needed to run a CAN example with dsPIC33CK256MP508 Curiosity board. The hardware consist of MCP2551 connected to the Curiosity board with click board (proto board). It is mounted on Click board - A of curiosity board.

<h1>Hardware Connection</h1>	
- MCP2551 is powered by +5V (its not 3.3V compatible)

- So, the RXD pin (MCP2551 --> dsPIC) voltage is converted from 5V to 3.3V level using the combination of 1K and 2K resistor. The the 3.3V logic is conncted to RP78/D14 pin.
- TXD pin (dsPIC --> MCP2551) voltage need not be converted from 3.3V to 5V, as the 3.3V logic is acceptable to MCP2551. Hence, TXD pin of MCP2551 is connected to RP70/D6 of dsPIC with a 330ohms resistor.
- Following picture shows the connection:

![Connection diagram](/hardware/connections_1.jpg)

<h1>Picture of proto board mounted on curiosity board</h1>	
- Following picture shows the prototype board with MCP2551, mounted on Click board-A of Curiosity board:

![Connection diagram](/hardware/connections_3.jpg)

<h1>Picture of connection of curiosity board with CAN Analyzer</h1>	
- Following picture shows the prototype board with MCP2551, connected with CAN Analyzer:

![Connection diagram](/hardware/connections_2.jpg)

<h1>Known issues</h1>

- Do not fully understand the FIFO working. Hence both TX and RX FIFO reading array starts from 8 to 15 (even though, the "byte" array is [8]"

- CAN clock is set to 40MHz, but the baud rate setting gives a clock frequency lower than 500kbps. But the setting used in the code works successfully with 500kbps setting of CAN bys analyzer.

- If proper baud rate of CAN bus is used (as understaood with calculation), the communication with CAN bus analyzer does not work.

- Receive is still in polling mode. Interrupt not yet tested.

<h1>Testing of code</h1>	
- Following picture shows the Code was tested in both transmit and receive mode with CAN bus abalyzer:

![Test window](/hardware/Test.png)
