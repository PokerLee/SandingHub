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
//! 			硬件环境：HX280025CEDC-LauchBoard开发套件
//!				连线方式：	绝对编码器 data->IO35  trigger->IO46
//! 						手轮 A->1A	B->1B
//! 						三相红黄黑偏置角 4.499354
//!
//! \author 李博克
//!
//! \date 2024-09-14
//!
//! \version V1.2
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
#include "sys_main.h"

//*****************************************************************************
// globals
//*****************************************************************************

volatile CPU_TIME_Obj     	cpuTime;
volatile CPU_TIME_Handle  	cpuTimeHandle;
volatile SYSTEM_Vars_t 		systemVars;
//uint32_t eqep_count = 0;
// 2.197912

//*****************************************************************************
// main function
//*****************************************************************************
// 2.2982163
int main(void)
{
	// 电机相关结构体指针赋值
	{
		cpuTimeHandle = (CPU_TIME_Handle)(&cpuTime);
		halHandle = (HAL_Handle)(&hal);
		motorHandle_M1 = (MOTOR_Handle)(&motorVars_M1);
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

	// set the reference speed, this can be replaced or removed
	motorVars_M1.flagEnableRunAndIdentify = false;
	motorVars_M1.speedRef_Hz = 10.0f;
	userParams_M1.flag_bypassMotorId =true;//false;//

	// 初始化电机1结构体
	initMotor1Handles(motorHandle_M1);

	// 初始化电机1控制参数
	initMotor1CtrlParameters(motorHandle_M1);

	motorVars_M1.flagEnableOffsetCalc = true;
	motorVars_M1.flagEnablePosCtrl = true;
	motorVars_M1.flagEnableSpeedCtrl = true;
	motorVars_M1.flagEnableCurrentCtrl = true;
	motorVars_M1.flagEnableAlignment = false;

	// 初始化CPU模块（计数器）

	CPU_TIME_reset(cpuTimeHandle);

	systemVars.flagEnableSystem = true;

	// 初始化中断向量表
	HAL_initIntVectorTable();

	// 启用ADC/PWM中断进行控制
	HAL_enableCtrlInts(halHandle);

	// 启用电机1零点偏移校准
	runMotor1OffsetsCalculation(motorHandle_M1);

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


//	   if((motorHandle_M1->fowardLevelCtrl!=1)&&(motorHandle_M1->backLevelCtrl!=1))
//	   {
//		   posCheckMoinitor(motorHandle_M1);
//	   }
//	   else
//	   {
//		   if(motorHandle_M1->fowardLevelCtrl == 1)
//		   {
//			   motorHandle_M1->backLevelCtrl = 0;
//			   if(!motorHandle_M1->flagLevelPosSet)
//			   {
//				   motorHandle_M1->posRef_rad = MATH_incrAngle1(motorHandle_M1->pos_rad += MATH_PI, 0);
//				   motorHandle_M1->flagLevelPosSet = 1;
//			   }
//		   }
//
//		   if(motorHandle_M1->backLevelCtrl == 1)
//		   {
//			   motorHandle_M1->fowardLevelCtrl = 0;
//			   if(!motorHandle_M1->flagLevelPosSet)
//			   {
//				   motorHandle_M1->posRef_rad = MATH_incrAngle1(motorHandle_M1->pos_rad -= MATH_PI, 0);
//				   motorHandle_M1->flagLevelPosSet = 1;
//			   }
//		   }
//	   }

	   // 1ms time base
	   if(HAL_getCPUTimerStatus(halHandle, HAL_CPU_TIMER0))
	   {
		   HAL_clearCPUTimerFlag(halHandle, HAL_CPU_TIMER0);

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
				   // motor 1 protection check
				   runMotorMonitor(motorHandle_M1);
				   break;
			   case 2:
				   calculateRMSData(motorHandle_M1);
				   break;
			   case 3:
//				   posCheckMoinitor(motorHandle_M1);
				   break;
			   case 4:
				   // calculate motor protection value
				   calcMotorOverCurrentThreshold(motorHandle_M1);
				   break;
			   case 5:
				   // system control
				   systemVars.timerBase_1ms = 0;
				   systemVars.timerCnt_5ms++;
				   break;
		   }
	   }

	   runMotor1Control(motorHandle_M1);

	}
	// disable the PWM
	HAL_disablePWM(motorHandle_M1->halMtrHandle);
}

//
// End of File
//

