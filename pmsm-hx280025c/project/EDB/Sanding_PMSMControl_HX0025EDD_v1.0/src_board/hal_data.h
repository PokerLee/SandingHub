//*****************************************************************************
//
//! \file hal_data.h
//!
//! \brief 外设数据结构相关
//!
//! \details
//!
//! \author 李博克
//!
//! \date 2024-09-14
//!
//! \version V1.1
//!
//! \copyright 北京三鼎光电仪器有限公司。
//
//*****************************************************************************

//*****************************************************************************
// includes
//*****************************************************************************

#ifndef HAL_DATA_H
#define HAL_DATA_H

#include "self_math.h"

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

//*****************************************************************************
//
//! \addtogroup HAL_DATA
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//! \brief Defines the ADC data
//
//*****************************************************************************
typedef struct _HAL_ADCData_t_
{
    float32_t VdcBus_V;         //!< the dc bus voltage values

    MATH_Vec3 I_A;              //!< the current values
    MATH_Vec3 V_V;              //!< the voltage values

    MATH_Vec3 offset_I_ad;      //!< the motor current offset value
    MATH_Vec3 offset_V_sf;      //!< the motor voltage offset factor

    float32_t current_sf;       //!< the current scale factor, amps/cnt
    float32_t voltage_sf;       //!< the phase voltage scale factor, volts/cnt
    float32_t dcBusvoltage_sf;  //!< the phase voltage scale factor, volts/cnt

} HAL_ADCData_t;

//*****************************************************************************
//
//! \brief Defines the PWM data
//
//*****************************************************************************
typedef struct _HAL_PWMData_t_
{
    MATH_Vec3 Vabc_pu;     //!< the PWM time-durations for each motor phase

    uint16_t  cmpValue[3];
    uint16_t  deadband;
    uint16_t  noiseWindow;
    uint16_t  minCMPValue;
    uint16_t  period;
    uint16_t  socCMP;
    bool      flagEnablePwm;
} HAL_PWMData_t;

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

#endif // end of HAL_DATA_H definition
