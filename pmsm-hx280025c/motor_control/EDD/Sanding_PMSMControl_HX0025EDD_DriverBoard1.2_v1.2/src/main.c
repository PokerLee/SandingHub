/******************************************************************************
 * Copyright (c) 2024 Beijing Sanding Technology All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Poker Lee
 * Email  : 1048402825@qq.com
 * FILE   : main.c
 *****************************************************************************/

//*****************************************************************************
//
//! \file main.c
//!
//! \brief 主执行函数
//!
//! \details
//! 			硬件环境：HX280025CEDD-LauchBoard开发套件
//!				连线方式：	绝对编码器 data->IO35  trigger->IO46
//! 						手轮 A->1A	B->1B
//! 						三相红黄黑偏置角 4.499354
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

#include <syscalls.h>
#include "IQmathLib.h"
#include "driverlib.h"
#include "device.h"
#include "communication.h"
#include "sys_main.h"

//*****************************************************************************
// globals
//*****************************************************************************

#define SHOW_MODE_START_POS	5.32028  //2.23018

//*****************************************************************************
// globals
//*****************************************************************************
// MOTOR
CODE_SECTION(".data")	volatile	MOTOR_Handle  	motorHandle_M1;
CODE_SECTION(".data")	volatile 	MOTOR_Vars_t 	motorVars_M1;
CODE_SECTION(".data")	volatile  	MOTOR_SetVars_t motorSetVars_M1;

// USER
CODE_SECTION(".data")	volatile 	USER_Params 	userParams_M1;

// DRIVER
CODE_SECTION(".data")	volatile 	HAL_MTR_Obj    	halMtr_M1;

// DRV8323S
CODE_SECTION(".data")	volatile  	DRVIC_VARS_t 	drvicVars_M1;

// ABS
#if MTR1_ABS_ENC
CODE_SECTION(".data")	volatile  	ENC_Obj			encM1;
#endif

// TRANSFORM
CODE_SECTION(".data")	volatile 	CLARKE_Obj    	clarke_V_M1;
CODE_SECTION(".data")	volatile  	CLARKE_Obj    	clarke_I_M1;
CODE_SECTION(".data")	volatile  	IPARK_Obj     	ipark_V_M1;
CODE_SECTION(".data")	volatile  	PARK_Obj      	park_I_M1;
CODE_SECTION(".data")	volatile  	PARK_Obj      	park_V_M1;

// SVPWM
CODE_SECTION(".data")	volatile  	SVGEN_Obj     	svgen_M1;

// PI CONTROLLER
CODE_SECTION(".data")	volatile  	PI_Obj        	pi_Id_M1;
CODE_SECTION(".data")	volatile  	PI_Obj        	pi_Iq_M1;
CODE_SECTION(".data")	volatile  	PI_Obj        	pi_spd_M1;
CODE_SECTION(".data")   volatile	PI_Obj        	pi_pos_M1;

// RAMP
CODE_SECTION(".data")	volatile  	TRAJ_Obj     	traj_spd_M1;

// SYSTEM
CODE_SECTION(".data")  	volatile 	SYSTEM_Vars_t 	systemVars;

//CODE_SECTION(".data")  	volatile 	uint8_t 		receivedChar[5];
//CODE_SECTION(".data")  	volatile 	uint8_t 		*p_receivedChar;
//*****************************************************************************
// main function
//*****************************************************************************

int main(void)
{
	// 电机相关结构体指针赋值
	{
		halHandle = (HAL_Handle)(&hal);
		motorHandle_M1 = (MOTOR_Handle)(&motorVars_M1);
//		p_receivedChar = receivedChar;
	}

	// 初始化设备时钟和外设
	Device_init();

	// 初始化通用IO引脚
	Device_initGPIO();

	// 初始化中断控制器
	Interrupt_initModule();

	// 初始化中断向量表
	Interrupt_initVectorTable();

	// 配置电机驱动外设地址
	HAL_init(halHandle);

	// 配置驱动电机外设模块参数
	HAL_setParams(halHandle);

	// CAN通讯
    // setup the CAN
    HAL_setupCANA(halHandle);

    // initialize the CANCOM
    initCANCOM();

    // setup the CAN interrupt
    HAL_enableCANInts(halHandle);

	// 初始化电机1结构体
	initMotor1Handles(motorHandle_M1);

	// 初始化电机1控制参数
	initMotor1CtrlParameters(motorHandle_M1);

	// set the reference speed, this can be replaced or removed
	motorHandle_M1->flagEnableRunAndIdentify = false;
	motorHandle_M1->speedRef_Rpm = 5.0f;
//	motorHandle_M1->speedRef_Hz = 0.24088715f;
	motorHandle_M1->posRef_rad = SHOW_MODE_START_POS;

    // set the control mode
	systemVars.flagEnableSystem = true;
	motorHandle_M1->flagEnableShowMode = false;
	motorHandle_M1->flagEnableOffsetCalc = false;
	motorHandle_M1->flagEnableAlignment = false;
	motorHandle_M1->brakingMode = DAMPING_BRAKE_MODE;
	motorHandle_M1->operateMode = OPERATE_MODE_IDLE;
	motorHandle_M1->uCtrlState = UCTRL_COARSE;
	motorHandle_M1->enablePosCtrl = true;
	motorHandle_M1->enableSpeedCtrl = true;
	motorHandle_M1->enableCurrentCtrl = true;
	motorHandle_M1->enableOpenLoopCtrl = false;
	// 演示模式
	motorHandle_M1->posRef_rad = SHOW_MODE_START_POS;
	motorHandle_M1->fowardLevelCtrl = 1;
	motorHandle_M1->backLevelCtrl = 0;
//	motorHandle_M1->angleDeltaSumCount = 500;
	// 启用电机1零点偏移校准
	runMotor1OffsetsCalculation(motorHandle_M1);

	// 初始化中断向量表
	HAL_initIntVectorTable();

	// 启用ADC/PWM中断进行控制
	HAL_enableCtrlInts(halHandle);

	// 启用全局中断
	HAL_enableGlobalInts(halHandle);

	// 启用调试中断
	HAL_enableDebugInt(halHandle);

	systemVars.powerRelayWaitTime_ms = POWER_RELAY_WAIT_TIME_ms;

	while(systemVars.flagEnableSystem == false)
	{
	   if(HAL_getCPUTimerStatus(halHandle, HAL_CPU_TIMER0))
	   {
		   HAL_clearCPUTimerFlag(halHandle, HAL_CPU_TIMER0);

		   systemVars.timerBase_1ms++;

		   if(systemVars.timerBase_1ms > systemVars.powerRelayWaitTime_ms)
		   {
			   systemVars.flagEnableSystem = true;
			   systemVars.timerBase_1ms = 0;
		   }
	   }
	}
	while(systemVars.flagEnableSystem == true)
	{
	   // loop while the enable system flag is true
	   systemVars.mainLoopCnt++;

	   if((motorHandle_M1->flagEnableShowMode == false)&&
			   (motorHandle_M1->enablePosCtrl == true))
	   {
		   posCheckMoinitor(motorHandle_M1);
	   }

	   // 1ms time base
	   if(HAL_getCPUTimerStatus(halHandle, HAL_CPU_TIMER0))
	   {
		   HAL_clearCPUTimerFlag(halHandle, HAL_CPU_TIMER0);

		   if((motorHandle_M1->flagEnableShowMode == false)&&
				   (motorHandle_M1->enablePosCtrl == true))
		   {
//			   encoderHandwheelSpeedDetection(motorHandle_M1);
		   }

		   // toggle status LED
		   systemVars.counterLED++;
		   if(systemVars.counterLED > (uint16_t)(LED_BLINK_FREQ_Hz * 1000))
		   {
			   HAL_toggleLED( HAL_GPIO_LED2C);
			   systemVars.counterLED = 0;
		   }

		   systemVars.timerBase_1ms++;
		   switch(systemVars.timerBase_1ms)
		   {
			   case 1:
				   updateCANCmdFreq(motorHandle_M1);
				   break;
			   case 2:
				   // motor 1 protection check
				   runMotorMonitor(motorHandle_M1);
				   break;
			   case 3:
				   calculateRMSData(motorHandle_M1);
				   break;
			   case 4:
				   // calculate motor protection value
				   calcMotorOverCurrentThreshold(motorHandle_M1);
				   break;
			   case 5:
				   // system control
//				   systemVars.timerBase_1ms = 0;
				   systemVars.timerCnt_5ms++;
				   break;
		   }
	   }

//       if((motorVars_M1.cmdCAN.flagEnablCmd == true) && (motorVars_M1.faultMtrUse.all == 0))
//       {
//           canComVars.flagCmdTxRun = motorVars_M1.cmdCAN.flagCmdRun;
//           canComVars.speedSet_Hz = motorVars_M1.cmdCAN.speedSet_Hz;
//
//           if(motorVars_M1.cmdCAN.flagEnablSyncLead == true)
//           {
//               motorVars_M1.flagEnableRunAndIdentify = motorVars_M1.cmdCAN.flagCmdRun;
//               motorVars_M1.speedRef_Hz = motorVars_M1.cmdCAN.speedSet_Hz;
//           }
//           else
//           {

//               motorVars_M1.flagEnableRunAndIdentify = canComVars.flagCmdRxRun;

//               motorVars_M1.speedRef_Hz = canComVars.speedRef_Hz;
//           }
//       }

	   if(motorHandle_M1->flagEnableShowMode == false)
	   {
		   if(systemVars.timerBase_1ms >= 5)
		   {
			   systemVars.timerBase_1ms = 0;
		   }
	   }
	   else if(systemVars.timerBase_1ms >= 7000) // 7s
	   {
		   if(motorHandle_M1->fowardLevelCtrl)
		   {
			   motorHandle_M1->posRef_rad = 5.35;
			   motorHandle_M1->fowardLevelCtrl = 0;
			   motorHandle_M1->backLevelCtrl = 1;
		   }
		   else if(motorHandle_M1->backLevelCtrl)
		   {
			   motorHandle_M1->posRef_rad = SHOW_MODE_START_POS;
			   motorHandle_M1->fowardLevelCtrl = 1;
			   motorHandle_M1->backLevelCtrl = 0;
		   }

		   systemVars.timerBase_1ms = 0;
	   }

	   runMotor1Control(motorHandle_M1);

	}
	// disable the PWM
	HAL_disablePWM(motorHandle_M1->halMtrHandle);
}


//
// End of File
//

