//###########################################################################
//
// FILE:    __enable_interrupts.S
//
// TITLE:   DSP28002x  __enable_interrupts Initialization & Support Functions.
//
//###########################################################################

.section .text, "ax", @progbits

.global __enable_interrupts

__enable_interrupts:

csrrsi a0,mstatus,8

ret

.size  __enable_interrupts,   .-__enable_interrupts
