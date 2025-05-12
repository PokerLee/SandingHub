//###########################################################################
//
// FILE:   SysCtl_delay.S
//
// TITLE:  H28x SysCtl_delay driver.
//
//###########################################################################

.section  ramfuncs, "ax", @progbits

.global	 SysCtl_delay


SysCtl_delay:

.align 1
	addi  sp,sp,-20
	sw    a1,16(sp) 
	csrr  a1,0x7c0
	sw    a1,12(sp)
	csrr  a1,mstatus
	
.align 2
	rpt a0,4
	nop
   
	csrw  mstatus,a1
	lw    a1,12(sp)
	csrw  0x7c0,a1
	lw    a1,16(sp)
	addi  sp,sp,20

	ret
	
.size  SysCtl_delay,   .-SysCtl_delay
