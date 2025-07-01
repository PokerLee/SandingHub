//*****************************************************************************
//
//! \file sys_main.h
//!
//! \brief 主执行函数头文件
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

#ifndef SYS_MAIN_H_
#define SYS_MAIN_H_

#include "motor_common.h"
#include "motor1_drive.h"

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
// defines
//*****************************************************************************

#define LED_BLINK_FREQ_Hz           (0.5f)       // 1Hz
#define POWER_RELAY_WAIT_TIME_ms    (1000)       // 1s

//*****************************************************************************
// structures
//*****************************************************************************

typedef struct _SYSTEM_Vars_t_
{
    uint32_t mainLoopCnt;

    uint16_t timerBase_1ms;
    uint16_t timerCnt_5ms;
    uint16_t timerCnt_1s;

    uint16_t powerRelayWaitTime_ms;

    uint16_t counterLED;        //!< Counter used to divide down the ISR rate for
                                //!< visually blinking an LED

    bool flagEnableSystem;
}SYSTEM_Vars_t;

//*****************************************************************************
// function prototypes
//*****************************************************************************
// DRIVER
extern	volatile 	HAL_MTR_Obj    	halMtr_M1;

// DRV8323S
extern	volatile  	DRVIC_VARS_t 	drvicVars_M1;

// ABS
#if MTR1_ABS_ENC
extern	volatile  	ENC_Obj			encM1;
#endif

// TRANSFORM
extern	volatile 	CLARKE_Obj    	clarke_V_M1;
extern	volatile  	CLARKE_Obj    	clarke_I_M1;
extern	volatile  	IPARK_Obj     	ipark_V_M1;
extern	volatile  	PARK_Obj      	park_I_M1;
extern	volatile  	PARK_Obj      	park_V_M1;

// SVPWM
extern	volatile  	SVGEN_Obj     	svgen_M1;

// PI CONTROLLER
extern	volatile  	PI_Obj        	pi_Id_M1;
extern	volatile  	PI_Obj        	pi_Iq_M1;
extern	volatile  	PI_Obj        	pi_spd_M1;
extern	volatile  	PI_Obj        	pi_pos_M1;

// RAMP
extern	volatile  	TRAJ_Obj     	traj_spd_M1;

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* SYS_MAIN_H_ */
