#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "clock.h"
#include "can.h"

#define CAN_COUNTER 10
uint16_t CANCountTemp = 0;

int main ( void )
{
    InitOscillator();
    
    TRISDbits.TRISD6 = 0;   // Output for CAN TX
    TRISDbits.TRISD14 = 1;  // Input for CAN RX
    _CAN1RXR = 78;      // CAN RX = RP78/PCI21/RD14
    _RP70R = 21;        // CAN TX = RP70/PMD14/RD6
    
    InitializeCAN1();
    transmitCAN1();    
    
    
    
    while(1)
    {
//        CANCountTemp++;
//        if (!(CANCountTemp < CAN_COUNTER))
//        {
//             CANCountTemp  = 0;
//             transmitCAN1();
//        }   
        receiveCAN1();
    }
}