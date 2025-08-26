/*
 * enc.h
 *
 *  Created on: 2025年6月10日
 *      Author: sddev
 */

#ifndef SRC_FOC_ENC_H_
#define SRC_FOC_ENC_H_



//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

#include "hw_types.h"
#include "hx_intrinsics.h"
#include "self_math.h"

#if MTR1_ABS_ENC
// **************************************************************************
// the defines

#define ENC_DEPTH	5 // 绝对编码器接收端深度

// **************************************************************************
// the typedefs

//! \brief Defines the ENC controller object
//!
typedef struct _ENC_Obj_
{
	uint32_t isrCnt;
	uint16_t receivedData[ENC_DEPTH];
	float32_t thetaMech_deg;
	float32_t thetaMech_rad;
} ENC_Obj;

//! \brief Defines the ENC handle
//!
typedef struct _ENC_Obj_ *ENC_Handle;

// **************************************************************************
// the function prototypes
static inline float32_t ENC_getThetaMechDeg(ENC_Handle handle)
{
    ENC_Obj *obj = (ENC_Obj *)handle;

    return(obj->thetaMech_deg);
}

static inline float32_t ENC_getThetaMechRad(ENC_Handle handle)
{
    ENC_Obj *obj = (ENC_Obj *)handle;

    return(obj->thetaMech_rad);
}

static inline void ENC_setThetaMechDeg(ENC_Handle handle)
{
    ENC_Obj *obj = (ENC_Obj *)handle;

    // Gets the current mechanical angular position of the motor
    obj->thetaMech_deg = (float32_t)((((obj->receivedData[1]<<16)&0xFF0000)|((obj->receivedData[2]<<8)&0xFF00)|(obj->receivedData[3]&0xFF))/36000.0f);
    return;
}

static inline void ENC_setThetaMechRad(ENC_Handle handle)
{
    ENC_Obj *obj = (ENC_Obj *)handle;

    obj->thetaMech_rad = obj->thetaMech_deg * MATH_PI / 180.0f;

    return;
}

#endif

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif



#endif /* SRC_FOC_ENC_H_ */
