//###########################################################################
//
// FILE:   cputimer.c
//
// TITLE:   H28x CPU timer Driver
//
//###########################################################################

#include "cputimer.h"

//*****************************************************************************
//
// CPUTimer_setEmulationMode
//
//*****************************************************************************
void CPUTimer_setEmulationMode(uint32_t base, CPUTimer_EmulationMode mode)
{
    ASSERT(CPUTimer_isBaseValid(base));
    //
    // Write to FREE_SOFT bits of register TCR
    //
      HWREG(base + CPUTIMER_O_TCR) =
            (HWREG(base + CPUTIMER_O_TCR) &
            ~(CPUTIMER_TCR_FREE | CPUTIMER_TCR_SOFT)) |
            (uint32_t)mode;
}

