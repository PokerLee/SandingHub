//#############################################################################
//
// $Copyright:
// Copyright (C) 2019-2024 Beijing Haawking Technology Co.,Ltd
// http://www.haawking.com/ All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions 
// are met:
// 
//   Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the 
//   documentation and/or other materials provided with the   
//   distribution.
// 
//   Neither the name of Beijing Haawking Technology Co.,Ltd nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
//#############################################################################


#ifndef HW_SCI_H
#define HW_SCI_H

//*************************************************************************************************
//
// The following are defines for the SCI register offsets
//
//*************************************************************************************************
#define SCI_O_CCR       0x0U    // Communications control register
#define SCI_O_CTL1      0x4U    // Control register 1
#define SCI_O_HBAUD     0x8U    // Baud rate (high) register
#define SCI_O_LBAUD     0xCU    // Baud rate (low) register
#define SCI_O_CTL2      0x10U   // Control register 2
#define SCI_O_RXST      0x14U   // Receive status register
#define SCI_O_RXEMU     0x18U   // Receive emulation buffer register
#define SCI_O_RXBUF     0x1CU   // Receive data buffer
#define SCI_O_TXBUF     0x20U   // Transmit data buffer
#define SCI_O_FFTX      0x24U   // FIFO transmit register
#define SCI_O_FFRX      0x28U   // FIFO receive register
#define SCI_O_FFCT      0x2CU   // FIFO control register
#define SCI_O_PRI       0x30U   // SCI priority control
#define SCI_O_RXTOCTL   0x34U   // SCI Receive time out Control Registers
#define SCI_O_RXTOCNT   0x38U   // SCI Receive time out count Registers


//*************************************************************************************************
//
// The following are defines for the bit fields in the SCICCR register
//
//*************************************************************************************************
#define SCI_CCR_SCICHAR_S       0U
#define SCI_CCR_SCICHAR_M       0x7U    // Character length control
#define SCI_CCR_ADDRIDLE_MODE   0x8U    // ADDR/IDLE Mode control
#define SCI_CCR_LOOPBKENA       0x10U   // Loop Back enable
#define SCI_CCR_PARITYENA       0x20U   // Parity enable
#define SCI_CCR_PARITY          0x40U   // Even or Odd Parity
#define SCI_CCR_STOPBITS        0x80U   // Number of Stop Bits

//*************************************************************************************************
//
// The following are defines for the bit fields in the SCICTL1 register
//
//*************************************************************************************************
#define SCI_CTL1_RXENA         0x1U    // SCI receiver enable
#define SCI_CTL1_TXENA         0x2U    // SCI transmitter enable
#define SCI_CTL1_SLEEP         0x4U    // SCI sleep
#define SCI_CTL1_TXWAKE        0x8U    // Transmitter wakeup method
#define SCI_CTL1_SWRESET       0x20U   // Software reset
#define SCI_CTL1_RXERRINTENA   0x40U   // Receive error interrupt enable

//*************************************************************************************************
//
// The following are defines for the bit fields in the SCIHBAUD register
//
//*************************************************************************************************
#define SCI_HBAUD_BAUD_S   0U
#define SCI_HBAUD_BAUD_M   0xFFU   // SCI 16-bit baud selection Registers SCIHBAUD

//*************************************************************************************************
//
// The following are defines for the bit fields in the SCILBAUD register
//
//*************************************************************************************************
#define SCI_LBAUD_BAUD_S   0U
#define SCI_LBAUD_BAUD_M   0xFFU   // SCI 16-bit baud selection Registers SCILBAUD

//*************************************************************************************************
//
// The following are defines for the bit fields in the SCICTL2 register
//
//*************************************************************************************************
#define SCI_CTL2_TXINTENA     0x1U    // Transmit __interrupt enable
#define SCI_CTL2_RXBKINTENA   0x2U    // Receiver-buffer break enable
#define SCI_CTL2_TXEMPTY      0x40U   // Transmitter empty flag
#define SCI_CTL2_TXRDY        0x80U   // Transmitter ready flag

//*************************************************************************************************
//
// The following are defines for the bit fields in the SCIRXST register
//
//*************************************************************************************************
#define SCI_RXST_RXWAKE    0x2U    // Receiver wakeup detect flag
#define SCI_RXST_PE        0x4U    // Parity error flag
#define SCI_RXST_OE        0x8U    // Overrun error flag
#define SCI_RXST_FE        0x10U   // Framing error flag
#define SCI_RXST_BRKDT     0x20U   // Break-detect flag
#define SCI_RXST_RXRDY     0x40U   // Receiver ready flag
#define SCI_RXST_RXERROR   0x80U   // Receiver error flag

//*************************************************************************************************
//
// The following are defines for the bit fields in the SCIRXEMU register
//
//*************************************************************************************************
#define SCI_RXEMU_ERXDT_S   0U
#define SCI_RXEMU_ERXDT_M   0xFFU   // Receive emulation buffer data

//*************************************************************************************************
//
// The following are defines for the bit fields in the SCIRXBUF register
//
//*************************************************************************************************
#define SCI_RXBUF_SAR_S     0U
#define SCI_RXBUF_SAR_M     0xFFU     // Receive Character bits
#define SCI_RXBUF_SCIFFPE   0x4000U   // Receiver error flag
#define SCI_RXBUF_SCIFFFE   0x8000U   // Receiver error flag

//*************************************************************************************************
//
// The following are defines for the bit fields in the SCITXBUF register
//
//*************************************************************************************************
#define SCI_TXBUF_TXDT_S   0U
#define SCI_TXBUF_TXDT_M   0xFFU   // Transmit data buffer

//*************************************************************************************************
//
// The following are defines for the bit fields in the SCIFFTX register
//
//*************************************************************************************************
#define SCI_FFTX_TXFFIL_S      0U
#define SCI_FFTX_TXFFIL_M      0x1FU     // Interrupt level
#define SCI_FFTX_TXFFIENA      0x20U     // Interrupt enable
#define SCI_FFTX_TXFFINTCLR    0x40U     // Clear INT flag
#define SCI_FFTX_TXFFINT       0x80U     // INT flag
#define SCI_FFTX_TXFFST_S      8U
#define SCI_FFTX_TXFFST_M      0x1F00U   // FIFO status
#define SCI_FFTX_TXFIFORESET   0x2000U   // FIFO reset
#define SCI_FFTX_SCIFFENA      0x4000U   // Enhancement enable
#define SCI_FFTX_SCIRST        0x8000U   // SCI reset rx/tx channels

//*************************************************************************************************
//
// The following are defines for the bit fields in the SCIFFRX register
//
//*************************************************************************************************
#define SCI_FFRX_RXFFIL_S      0U
#define SCI_FFRX_RXFFIL_M      0x1FU     // Interrupt level
#define SCI_FFRX_RXFFIENA      0x20U     // Interrupt enable
#define SCI_FFRX_RXFFINTCLR    0x40U     // Clear INT flag
#define SCI_FFRX_RXFFINT       0x80U     // INT flag
#define SCI_FFRX_RXFFST_S      8U
#define SCI_FFRX_RXFFST_M      0x1F00U   // FIFO status
#define SCI_FFRX_RXFIFORESET   0x2000U   // FIFO reset
#define SCI_FFRX_RXFFOVRCLR    0x4000U   // Clear overflow
#define SCI_FFRX_RXFFOVF       0x8000U   // FIFO overflow

//*************************************************************************************************
//
// The following are defines for the bit fields in the SCIFFCT register
//
//*************************************************************************************************
#define SCI_FFCT_FFTXDLY_S   0U
#define SCI_FFCT_FFTXDLY_M   0xFFU     // FIFO transmit delay
#define SCI_FFCT_CDC         0x2000U   // Auto baud mode enable
#define SCI_FFCT_ABDCLR      0x4000U   // Auto baud clear
#define SCI_FFCT_ABD         0x8000U   // Auto baud detect

//*************************************************************************************************
//
// The following are defines for the bit fields in the SCIPRI register
//
//*************************************************************************************************
#define SCI_PRI_FREESOFT_S   3U
#define SCI_PRI_FREESOFT_M   0x18U   // Emulation modes

//*************************************************************************************************
//
// The following are defines for the bit fields in the SCIRXTOCTL register
//
//*************************************************************************************************
#define SCI_RXTOCTL_TO      0x1U      // SCIRX timeout occured flag
#define SCI_RXTOCTL_INTEN   0x4000U   // SCIRX timeout interrutp enable
#define SCI_RXTOCTL_TOEN    0x8000U   // SCIRX timeout detect enable

//*************************************************************************************************
//
// The following are defines for the bit fields in the SCIRXTOCNT register
//
//*************************************************************************************************
#define SCI_RXTOCNT_RXTOCNT_S   0U
#define SCI_RXTOCNT_RXTOCNT_M   0xFFFFU   // SCIRX timeout conter config



#endif
