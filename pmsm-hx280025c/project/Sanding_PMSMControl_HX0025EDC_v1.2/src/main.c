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
//! \brief ��ִ�к���
//!
//! \details
//! 			Ӳ��������HX280025CEDC-LauchBoard�����׼�
//!				���߷�ʽ��	���Ա����� data->IO35  trigger->IO46
//! 						���� A->1A	B->1B
//! 						�����ƺ�ƫ�ý� 4.499354
//!
//! \author ���
//!
//! \date 2024-09-14
//!
//! \version V1.2
//!
//! \copyright ������������������޹�˾��
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
	// �����ؽṹ��ָ�븳ֵ
	{
		cpuTimeHandle = (CPU_TIME_Handle)(&cpuTime);
		halHandle = (HAL_Handle)(&hal);
		motorHandle_M1 = (MOTOR_Handle)(&motorVars_M1);
	}

	// ��ʼ���豸ʱ�Ӻ�����
	Device_init();

	// ��ʼ��ͨ��IO����
	Device_initGPIO();

	// ��ʼ���жϿ�����
	Interrupt_initModule();

	// ��ʼ���ж�������
	Interrupt_initVectorTable();

	// ���õ�����������ַ
	HAL_init(halHandle);

	// ���������������ģ�����
	HAL_setParams(halHandle);

	// set the reference speed, this can be replaced or removed
	motorVars_M1.flagEnableRunAndIdentify = false;
	motorVars_M1.speedRef_Hz = 10.0f;
	userParams_M1.flag_bypassMotorId =true;//false;//

	// ��ʼ�����1�ṹ��
	initMotor1Handles(motorHandle_M1);

	// ��ʼ�����1���Ʋ���
	initMotor1CtrlParameters(motorHandle_M1);

	motorVars_M1.flagEnableOffsetCalc = true;
	motorVars_M1.flagEnablePosCtrl = true;
	motorVars_M1.flagEnableSpeedCtrl = true;
	motorVars_M1.flagEnableCurrentCtrl = true;
	motorVars_M1.flagEnableAlignment = false;

	// ��ʼ��CPUģ�飨��������

	CPU_TIME_reset(cpuTimeHandle);

	systemVars.flagEnableSystem = true;

	// ��ʼ���ж�������
	HAL_initIntVectorTable();

	// ����ADC/PWM�жϽ��п���
	HAL_enableCtrlInts(halHandle);

	// ���õ��1���ƫ��У׼
	runMotor1OffsetsCalculation(motorHandle_M1);

	// ����ȫ���ж�
	HAL_enableGlobalInts(halHandle);

	// ���õ����ж�
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

