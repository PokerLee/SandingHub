//###########################################################################
//
// FILE:   hw_types.h
//
// TITLE:  Type definitions used in driverlib functions.
//
//###########################################################################


#ifndef HW_TYPES_H
#define HW_TYPES_H

#include "hx_rv32_type.h"

//*****************************************************************************
//
// Macros for hardware access
//
//*****************************************************************************

#define CODE_SECTION(v) __attribute__((section(v)))

#define __byte(x,y)  (((uint8_t *)(x))[y])

#define HWREG(x)                                                              \
        (*((volatile uint32_t *)((uintptr_t)(x))))
#define HWREG_BP(x)                                                           \
	    (*((volatile uint32_t *)((uintptr_t)(x))))



//*****************************************************************************
//
// SUCCESS and FAILURE for API return value
//
//*****************************************************************************
#define STATUS_S_SUCCESS    (0)
#define STATUS_E_FAILURE    (-1)

//*****************************************************************************
//
// 32-bit and 64-bit float type
//
//*****************************************************************************

typedef float         float32_t;
typedef double        float64_t;

//****************************************************************************
//
// For checking NULL pointers
//
//****************************************************************************
#ifndef NULL
#define NULL ((void *)0x0)
#endif



#endif // HW_TYPES_H
