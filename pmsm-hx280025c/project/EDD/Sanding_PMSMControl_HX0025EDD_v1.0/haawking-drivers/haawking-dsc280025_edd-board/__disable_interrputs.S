//###########################################################################
//
// FILE:    __disable_interrputs.S
//
// TITLE:   DSP28002x  __disable_interrputs Initialization & Support Functions.
//
//###########################################################################

.section .text,  "ax", @progbits


.global __disable_interrupts


__disable_interrupts:

csrrci a0,mstatus,0x8
ret

.size  __disable_interrupts,   .-__disable_interrupts
