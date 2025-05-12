/*#############################################################################*/
/*                                                                             */
/* $Copyright:                                                                 */
/* Copyright (C) 2019-2024 Beijing Haawking Technology Co.,Ltd                 */
/* http://www.haawking.com/ All rights reserved.                               */
/*                                                                             */
/* Redistribution and use in source and binary forms, with or without          */
/* modification, are permitted provided that the following conditions          */
/* are met:                                                                    */
/*                                                                             */
/*   Redistributions of source code must retain the above copyright            */
/*   notice, this list of conditions and the following disclaimer.             */
/*                                                                             */
/*   Redistributions in binary form must reproduce the above copyright         */
/*   notice, this list of conditions and the following disclaimer in the       */
/*   documentation and/or other materials provided with the                    */
/*   distribution.                                                             */
/*                                                                             */
/*   Neither the name of Beijing Haawking Technology Co.,Ltd nor the names of  */
/*   its contributors may be used to endorse or promote products derived       */
/*   from this software without specific prior written permission.             */
/*                                                                             */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS         */
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT           */
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR       */
/* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT        */
/* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,       */
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT            */
/* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,       */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY       */
/* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT         */
/* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE       */
/* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.        */
/*                                                                             */
/*#############################################################################*/
/*                                                                             */
/* Release for HXS320F280025CEDD, Ram Linker, 1.0.0                            */
/*                                                                             */
/* Release time: 2024-07-29 13:12:55.057841                                    */
/*                                                                             */
/*#############################################################################*/

OUTPUT_ARCH("riscv")
ENTRY(_start)
INCLUDE "Peripheral.ld" 
INCLUDE "BootROM.ld" 

/*----------------------------------------------------------------------*/
/* Memories                                                             */
/*----------------------------------------------------------------------*/
MEMORY
{
    BEGIN(rwx):                    ORIGIN = 0x00000000, LENGTH = 0x00000010     /* Code Entry */
    RAMM0M1(rwx):                  ORIGIN = 0x00000010, LENGTH = 0x00000FF0     /* Internal RAM M0 to M1 */
    RAMLS4TO7(rwx):                ORIGIN = 0x00010000, LENGTH = 0x00008000     /* Internal RAM LS4 to LS7 */
    RAMGS0(rwx):                   ORIGIN = 0x00018000, LENGTH = 0x00004000     /* Internal RAM GS0 */
    FLASH(rwx):                    ORIGIN = 0x00600000, LENGTH = 0x00040000     /* Internal Flash Bank0 */
    USEROTP(rwx):                  ORIGIN = 0x007A0000, LENGTH = 0x00000800     /* User Otp for DCSM configuration */
    IER_REGISTER_FILE(rw):         ORIGIN = 0x00100C10, LENGTH = 0x00000004     /* IER Regitser */
    IFR_REGISTER_FILE(rw):         ORIGIN = 0x00100D10, LENGTH = 0x00000004     /* IFR Regitser */
}

/*----------------------------------------------------------------------*/
/* Sections                                                             */
/*----------------------------------------------------------------------*/
SECTIONS
{
    . = 0x00000000;
    /* Code start vector */
    codestart ALIGN(0x10):
    {
        *(codestart)
    } > BEGIN

    . = 0x00000010;
    /* Stack usage memory */
    stack(NOLOAD): ALIGN(0x10)
    {
        __global_sp$ = . + 0xFE8;
    } > RAMM0M1

    . = 0x00010000;
    /* Text segment */
    text ALIGN(0x10):
    {
        *(.text.init)
        *(.text)
        *(.text*)
        *(.IQmath.*)
        *(.srodata)
        *(.srodata*)
        *(.rodata)
        *(.rodata*)
    } > RAMLS4TO7

    /* Ram function segment */
    ramfunc ALIGN(0x10):
    {
        *(ramconsts)
        *(ramfuncs)
    } > RAMLS4TO7

    /* Data segment */
    data ALIGN(0x10):
    {
        __global_pointer$ = . + 0x800;
        *(.data)
        *(.data.*)
        *(.sdata)
        *(.sdata.*)
        *(.gnu.linkonce.s.*)
    } > RAMLS4TO7

    /* Bss segment */
    bss(NOLOAD): ALIGN(0x10)
    {
        __BSS_START = .;
        *(.sbss)
        *(.sbss.*)
        *(.scommon)
        *(.gnu.linkonce.sb.*)
        *(.bss)
        *(.bss.*)
        *(COMMON)
        __BSS_END = ALIGN(0x10);
    } > RAMLS4TO7

    . = 0x00018000;
    /* Data in ramgs0 */
    ramgs0 ALIGN(0x10):
    {
        *(ramgs0)
    } > RAMGS0

    . = 0x00100C10;
    /* IER csr register */
    ier_register(NOLOAD): ALIGN(0x10)
    {
        *(.ier_register)
    } > IER_REGISTER_FILE

    . = 0x00100D10;
    /* IFR csr register */
    ifr_register(NOLOAD): ALIGN(0x10)
    {
        *(.ifr_register)
    } > IFR_REGISTER_FILE

    /* End */
    _end = .;
}
