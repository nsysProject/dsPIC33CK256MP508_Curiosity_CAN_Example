/*******************************************************************************
  CAN Configuration Routine source File

  File Name:
    can.c

  Summary:
    This file includes subroutine for initializing CAN module of Controller

  Description:
    Definitions in the file are for dsPIC33CK256MP508 on Motor Control 
    Development board from Microchip

*******************************************************************************/
/*******************************************************************************
* Copyright (c) 2017 released Microchip Technology Inc.  All rights reserved.
*
* SOFTWARE LICENSE AGREEMENT:
* 
* Microchip Technology Incorporated ("Microchip") retains all ownership and
* intellectual property rights in the code accompanying this message and in all
* derivatives hereto.  You may use this code, and any derivatives created by
* any person or entity by or on your behalf, exclusively with Microchip's
* proprietary products.  Your acceptance and/or use of this code constitutes
* agreement to the terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
* TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE,
* WHETHER IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF
* STATUTORY DUTY),STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE,
* FOR ANY INDIRECT, SPECIAL,PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL
* LOSS, DAMAGE, FOR COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE CODE,
* HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR
* THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWABLE BY LAW,
* MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS CODE,
* SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO
* HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and
* determining its suitability.  Microchip has no obligation to modify, test,
* certify, or support the code.
*
*******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "can.h"
// *****************************************************************************
// *****************************************************************************
// Section: 
// *****************************************************************************
// *****************************************************************************
unsigned int __attribute__((aligned(4)))CanBuffer[MAX_WORDS];
// *****************************************************************************
// *****************************************************************************
// Section: 
// *****************************************************************************
// *****************************************************************************
/*data structure to implement a CANFD  message buffer. */
/* CANFD Message Time Stamp */
typedef unsigned long CANFD_MSG_TIMESTAMP;
/* CAN TX Message Object Control*/
typedef struct _CANFD_TX_MSGOBJ_CTRL {
    unsigned DLC:4;
    unsigned IDE:1;
    unsigned RTR:1;
    unsigned BRS:1;
    unsigned FDF:1;
    unsigned ESI:1;
    unsigned SEQ:7;
    unsigned unimplemented1:16;
} CANFD_TX_MSGOBJ_CTRL;

/* CANFD RX Message Object Control*/
typedef struct _CANFD_RX_MSGOBJ_CTRL {
    unsigned DLC:4;
    unsigned IDE:1;
    unsigned RTR:1;
    unsigned BRS:1;
    unsigned FDF:1;
    unsigned ESI:1;
    unsigned unimplemented1:2;
    unsigned FilterHit:5;
    unsigned unimplemented2:16;
} CANFD_RX_MSGOBJ_CTRL;

/* CANFD TX Message ID*/
typedef struct _CANFD_MSGOBJ_ID {
    unsigned SID:11;
    unsigned long EID:18;
    unsigned SID11:1;
    unsigned unimplemented1:2;
} CANFD_MSGOBJ_ID;

/* CAN TX Message Object*/
typedef union _CANFD_TX_MSGOBJ {
    struct  {
        CANFD_MSGOBJ_ID id;
        CANFD_TX_MSGOBJ_CTRL ctrl;
        CANFD_MSG_TIMESTAMP timeStamp;
    } bF;
    unsigned int word[4];
    unsigned char byte[8];
} CANFD_TX_MSGOBJ;

/* CANFD RX Message Object */
typedef union _CANFD_RX_MSGOBJ {
    struct  {
        CANFD_MSGOBJ_ID id;
        CANFD_RX_MSGOBJ_CTRL ctrl;
        CANFD_MSG_TIMESTAMP timeStamp;
    } bF;
    unsigned int word[4];
    unsigned char byte[8];
} CANFD_RX_MSGOBJ;
// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
void InitializeCAN1(void);
// *****************************************************************************
/* Function:
    void InitializeCAN1 (void)

  Summary:
    Routine to configure CAN Module

  Description:
    Function to configure CAN1 Module
  
  Precondition:
    None.

  Parameters:
    None

  Returns:
    None.

  Remarks:
    None.
 */
 void InitializeCAN1 (void)
 {
   /* Set up the CAN clock for 40 MHZ and enable the CAN clock generator. */

    CANCLKCONbits.CANCLKSEL = 2;	// FPLLO = 400MHz
    CANCLKCONbits.CANCLKDIV = 9;  	// div by 10, 400/10 = 40MHz
    CANCLKCONbits.CANCLKEN = 1;	 
	
    /* Enable the CANFD module */
    C1CONLbits.CON = 1;

    /* Place CAN module in configuration mode */
    C1CONHbits.REQOP = 4;
    while(C1CONHbits.OPMOD != 4);

    /* Initialize the C1FIFOBA with the start address of the CAN FIFO message buffer area. */ 
    C1FIFOBAL = (unsigned int) &CanBuffer;

    /* Set up the CANFD module for 500Kbps bit rate. */
 //   C1NBTCFGH = 0x003E;
 //   C1NBTCFGL = 0x0F0F;
    
    C1NBTCFGHbits.BRP = 0;      // 1:1 = 40MHz
// 40MHz/500,000 = 80 TQ.
// That means, if we have 80 pulses of 40Mhz, we will get CAN communication at 50kbps
// Sample point = 80% of 80 TQ = 64 TQ. 1 TQ is used for SJW, so TSEG1 should be 63 TQ
    C1NBTCFGLbits.SJW = 15;         // SJW = 16 TQ
    C1NBTCFGHbits.TSEG1 = 62;       // TSEG1 = 63 TQ
    C1NBTCFGLbits.TSEG2 = 15;       // TSEG2 = 16 TQ

//    C1NBTCFGHbits.BRP = 0;      // 1:1 = 40MHz
//// 40MHz/500,000 = 80 TQ.
//// That means, if we have 80 pulses of 40Mhz, we will get CAN communication at 50kbps
//// Sample point = 80% of 80 TQ = 64 TQ. 1 TQ is used for SJW, so TSEG1 should be 63 TQ
//    C1NBTCFGLbits.SJW = 0;      // SJW = 1 TQ
//    C1NBTCFGHbits.TSEG1 = 63;       // TSEG1 = 63 TQ
//    C1NBTCFGLbits.TSEG2 = 15;       // TSEG2 = 16 TQ


    
    
//================= Transmit configuration ==========================    
    /* Configure CANFD module to enable Transmit Queue and disable BRS*/  
    C1CONLbits.BRSDIS = 0x1; // diable BRS
    C1CONHbits.STEF = 0x0; //Don't save transmitted messages in TEF
    C1CONHbits.TXQEN = 0x1;

    /* Configure TXQ to transmit 1 message*/ 
    C1TXQCONHbits.FSIZE = 0x0; // single message
    C1TXQCONHbits.PLSIZE = 0x0; // 8 bytes of data

    /* Configure FIFO1 to transmit 2 messages*/
    C1FIFOCON1Hbits.FSIZE = 0x1; //2 messages
    C1FIFOCON1Hbits.PLSIZE = 0x0; //8 bytes of data
    C1FIFOCON1Lbits.TXEN = 0x1; // Set TXEN bit ,transmit fifo
    
//================== Receive configuration ==========================
    /* Configure FIFO2 to Receive 2 messages*/
    C1FIFOCON2Hbits.FSIZE = 0x1; //2 messages
    C1FIFOCON2Hbits.PLSIZE = 0x0; //8 bytes of data
    C1FIFOCON2Lbits.TXEN = 0x0; //Receive fifo    
    
    /* Configure filter 0 and MASK 0 to accept extended id messages with id = 2 and 3*/
    C1FLTCON0Lbits.F0BP = 2 ; // message stored in FIFO2
    C1FLTOBJ0Hbits.EXIDE = 0;   // Standard ID only
    
// standard ID used for receive is "0x784"    
    C1FLTOBJ0Hbits.SID11 = 1;   // Bit-11 of standard ID is one
    C1FLTOBJ0Lbits.SID = 0x384;
    
    C1MASK0Hbits.MIDE = 1;  // Mask format is decided by EXIDE state ( = standard ID)
    C1MASK0Hbits.MSID11 = 1;    // Bit-11 is not masked. Bit-11 has to exactly match with Bit-11 in filter to be accepted
    C1MASK0Lbits.MSID = 0x384;  

    C1FLTCON0Lbits.FLTEN0 = 1; // Enable the filter 0    
    

    /* Place the CAN module in legacy CAN2.0 mode. */
    C1CONHbits.REQOP = 6;
    while(C1CONHbits.OPMOD != 6); 
 }
 // *****************************************************************************
/* Function:
    void TransmitCAN1 (void)

  Summary:
    Routine to transmit one packet of standard CAN data

  Description:
    Function to transmit one packet of standard CAN data
  
  Precondition:
    None.

  Parameters:
    None

  Returns:
    None.

  Remarks:
    None.
 */
  void transmitCAN1 (void)
  {
     unsigned int index;
     
    /* Get the address of the message buffer to write to. Load the buffer and */
    /* then set the UINC bit. Set the TXREQ bit next to send the message. */

    CANFD_TX_MSGOBJ *txObj; 

    /* Transmit message 0 from FIFO 1 */
    /* SID = 0x300 , 8 bytes of data */
    txObj = (CANFD_TX_MSGOBJ *)C1FIFOUA1L;   
    txObj->bF.id.SID = 0x300;
    txObj->bF.id.EID = 0x0000;
    txObj->bF.ctrl.BRS = 0 ;  //do not switch bit rate
    txObj->bF.ctrl.DLC = 0x8; //8bytes
    txObj->bF.ctrl.FDF = 0; // CAN2.0 frame
    txObj->bF.ctrl.IDE = 0; //Standard frame

    for (index=0;index<0x8;index++ )
    {
       txObj->byte[index+8] = 0xA5 ; // 8 bytes of 0xA5
    }
    C1FIFOCON1Lbits.UINC = 1; // Set UINC bit
    C1FIFOCON1Lbits.TXREQ = 1; // Set TXREQ bit 

    /* Transmit message 1 from FIFO 1*/
    /* SID = 0x500, EID = 0x0000, 12 bytes of data */
    txObj = (CANFD_TX_MSGOBJ *)C1FIFOUA1L;   
    txObj->bF.id.SID = 0x500;
    txObj->bF.id.EID = 0x0000;
    txObj->bF.ctrl.BRS = 0 ;  //do not switch bit rate
    txObj->bF.ctrl.DLC = 0x8; //8 bytes
    txObj->bF.ctrl.FDF = 0; // CAN2.0 frame
    txObj->bF.ctrl.IDE = 0; //Standard frame

    for (index=0;index<0x8;index++ )
    {
       txObj->byte[index+8] = 0x55 ; // 8 bytes of 0x55
    }
    C1FIFOCON1Lbits.UINC = 1; // Set UINC bit
    C1FIFOCON1Lbits.TXREQ = 1; // Set TXREQ bit 
  }
   // *****************************************************************************
/* Function:
    void receiveCAN1 (void)

  Summary:
    Routine to receive one packet of standard CAN data

  Description:
    Function to receive one packet of standard CAN data
  
  Precondition:
    None.

  Parameters:
    None

  Returns:
    None.

  Remarks:
    None.
 */
CANFD_RX_MSGOBJ rxBuffer;
CANFD_RX_MSGOBJ *rxObj = &rxBuffer; 
uint8_t data[8];  

  void receiveCAN1 (void)
  {
    /* Get the address of the message buffer to read the received messages.*/   
    /* set UINC bit to update the FIFO tail*/

    
    rxObj = (CANFD_RX_MSGOBJ *)C1FIFOUA2L;  
    while(C1FIFOSTA2bits.TFNRFNIF ==0); 
    
    
    data[0] = rxObj->byte[8];
    data[1] = rxObj->byte[9];
    data[2] = rxObj->byte[10];
    data[3] = rxObj->byte[11];
    data[4] = rxObj->byte[12];
    data[5] = rxObj->byte[13];
    data[6] = rxObj->byte[14];
    data[7] = rxObj->byte[15];     
    
    //Process the received messages 
    C1FIFOCON2Lbits.UINC = 1; // Update the FIFO message pointer.      






//	uint16_t * address;
//
//    /* Get the address of the message buffer to read the received messages.*/   
//    /* set UINC bit to update the FIFO tail*/
//    CANFD_RX_MSGOBJ *rxObj; 
//    
//    address = (uint16_t *) &C1FIFOUA2L;  
//    
//    
//    while(C1FIFOSTA2bits.TFNRFNIF ==0); 
//    //Process the received messages 
//    C1FIFOCON2Lbits.UINC = 1; // Update the FIFO message pointer. 
  }