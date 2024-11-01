/*
 * Copyright (c) 2019-2022 Beijing Haawking Technology Co.,Ltd
 * All rights reserved.
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
 *****************************************************************/
 
#include <syscalls.h>
#include "IQmathLib.h"
#include "driverlib.h"
#include "device.h"
#include "sys_main.h"

CPU_TIME_Obj     cpuTime;
CPU_TIME_Handle  cpuTimeHandle;
volatile SYSTEM_Vars_t systemVars;

// ¾ø¶Ô±àÂëÆ÷ data->IO35  trigger->IO46
// ÊÖÂÖ A->1A	B->1B
// ÈýÏàºì»ÆºÚÆ«ÖÃ½Ç 4.499354

uint32_t eqep_count = 0;
int main(void)
{
	//Initialize device clock and perioherals
 	Device_init();

 	//Disable pin locks and enable interal pullups
	Device_initGPIO();

    // initialize the interrupt controller
    Interrupt_initModule();

    // init vector table
    Interrupt_initVectorTable();

	halHandle = HAL_init(&hal);

	// set the driver parameters
	HAL_setParams(halHandle);

    // set the control parameters for motor 1
    motorHandle_M1 = (MOTOR_Handle)(&motorVars_M1);

    // set the reference speed, this can be replaced or removed
    motorVars_M1.flagEnableRunAndIdentify = false;
    motorVars_M1.speedRef_Hz = 10.0f;
    userParams_M1.flag_bypassMotorId =true;//false;//

    // initialize motor control parameters
    initMotor1Handles(motorHandle_M1);
    initMotor1CtrlParameters(motorHandle_M1);

    motorVars_M1.flagEnableOffsetCalc = true;
    motorVars_M1.flagEnablePosCtrl = true;
    motorVars_M1.flagEnableSpeedCtrl = true;
    motorVars_M1.flagEnableCurrentCtrl = true;
    motorVars_M1.flagEnableAlignment = false;

    // initialize the CPU usage module
    cpuTimeHandle = &cpuTime;
    CPU_TIME_reset(cpuTimeHandle);

    systemVars.flagEnableSystem = true;

    // initialize the interrupt vector table
    HAL_initIntVectorTable();

    // enable the ADC/PWM interrupts for control
    // enable interrupts to trig DMA
    HAL_enableCtrlInts(halHandle);

//    motorVars_M1.flagEnableRsOnLine = true;
//    motorVars_M1.flagEnableRsRecalc = true;

    // run offset calibration for motor 1
    runMotor1OffsetsCalculation(motorHandle_M1);

    // enable global interrupts
    HAL_enableGlobalInts(halHandle);

    // enable debug interrupts
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
               case 1:     // motor 1 protection check
                  runMotorMonitor(motorHandle_M1);
                   break;
               case 2:
                   calculateRMSData(motorHandle_M1);
                   break;
               case 3:
            	   eqep_count = EQEP_getPosition(MTR1_EQEP_ENCODER_BASE);
            	   motorVars_M1.posRef_rad = (float32_t)(eqep_count/1000.0f);
                   break;
               case 4:     // calculate motor protection value
                   calcMotorOverCurrentThreshold(motorHandle_M1);
                   break;
               case 5:     // system control
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

// Current loop test
// PI
// Iq: Kp 0.1 Ki 1.0 // Id: Kp 0.1 Ki 1.0 -> err:0.015
// spd:Kp 0.8 ki 0.01


//
// End of File
//

