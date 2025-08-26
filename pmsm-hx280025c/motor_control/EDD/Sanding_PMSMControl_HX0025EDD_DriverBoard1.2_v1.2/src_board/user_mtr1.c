//*****************************************************************************
//
//! \file user_mtr1.c
//!
//! \brief 用户电机参数相关
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

#include "user_mtr1.h"
#include "hal.h"

//*****************************************************************************
//
// USER_setParams, set control parameters for motor 1
//
//*****************************************************************************
void USER_setMotor1Params(USER_Params *pUserParams)
{
    pUserParams->dcBus_nominal_V = USER_M1_NOMINAL_DC_BUS_VOLTAGE_V;

    pUserParams->numIsrTicksPerCtrlTick = 1;
    pUserParams->numIsrTicksPerTrajTick = 1;
    pUserParams->numCtrlTicksPerCurrentTick = USER_M1_NUM_ISR_TICKS_PER_CURRENT_TICK;
    pUserParams->numCtrlTicksPerSpeedTick = USER_M1_NUM_ISR_TICKS_PER_SPEED_TICK;
    pUserParams->numCtrlTicksPerPosTick = USER_M1_NUM_ISR_TICKS_PER_POS_TICK;

    pUserParams->numCurrentSensors = USER_M1_NUM_CURRENT_SENSORS;
    pUserParams->numVoltageSensors = USER_M1_NUM_VOLTAGE_SENSORS;

    pUserParams->systemFreq_MHz = USER_SYSTEM_FREQ_MHz;

    pUserParams->pwmPeriod_Hz = USER_M1_PWM_FREQ_kHz * 1000.0f;
    pUserParams->pwmPeriod_usec = USER_M1_PWM_PERIOD_usec;

    pUserParams->voltage_sf = USER_M1_VOLTAGE_SF;

    pUserParams->current_sf = USER_M1_CURRENT_SF;

    pUserParams->dcBusPole_rps = USER_M1_DCBUS_POLE_rps;

    pUserParams->speedPole_rps = USER_M1_SPEED_POLE_rps;

    pUserParams->maxVsMag_pu = USER_M1_MAX_VS_MAG_PU;

    pUserParams->motor_type = USER_MOTOR1_TYPE;

    pUserParams->motor_numPolePairs = USER_MOTOR1_NUM_POLE_PAIRS;

    pUserParams->motor_numEncSlots = USER_MOTOR1_NUM_ENC_SLOTS;

    pUserParams->motor_ratedFlux_Wb = USER_MOTOR1_RATED_FLUX_VpHz / MATH_TWO_PI;

    pUserParams->motor_Rr_Ohm = USER_MOTOR1_Rr_Ohm;
    pUserParams->motor_Rs_Ohm = USER_MOTOR1_Rs_Ohm;

    pUserParams->motor_Ls_d_H = USER_MOTOR1_Ls_d_H;
    pUserParams->motor_Ls_q_H = USER_MOTOR1_Ls_q_H;

    pUserParams->maxCurrent_A = USER_MOTOR1_MAX_CURRENT_A;

    pUserParams->IdRated_A = USER_MOTOR1_MAGNETIZING_CURRENT_A;

    pUserParams->Vd_sf = USER_M1_VD_SF;
    pUserParams->maxVsMag_V = USER_MOTOR1_RATED_VOLTAGE_V;

    pUserParams->angleDelayed_sf_sec = (float32_t)0.5f * USER_M1_CTRL_PERIOD_sec;

    pUserParams->fluxExcFreq_Hz = USER_MOTOR1_FLUX_EXC_FREQ_Hz;

    pUserParams->ctrlFreq_Hz = USER_M1_ISR_FREQ_Hz;

    pUserParams->trajFreq_Hz = USER_M1_ISR_FREQ_Hz;

    pUserParams->ctrlPeriod_sec = USER_M1_CTRL_PERIOD_sec;

    pUserParams->maxAccel_Hzps = USER_M1_MAX_ACCEL_Hzps;

    pUserParams->maxFrequency_Hz = USER_MOTOR1_FREQ_MAX_HZ;

    return;
} // end of USER_setParams() function
