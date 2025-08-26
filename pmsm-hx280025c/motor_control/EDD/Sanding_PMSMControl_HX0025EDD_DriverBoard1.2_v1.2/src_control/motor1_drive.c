//*****************************************************************************
//
//! \file motor1_drive.c
//!
//! \brief 电机控制结构体相关
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

#include "sys_main.h"
#include "always.h"
#include "motor1_drive.h"

//*****************************************************************************
// function prototypes
//*****************************************************************************

void initMotor1Handles(MOTOR_Handle handle)
{
    MOTOR_Vars_t *obj = (MOTOR_Vars_t *)handle;

    // initialize the driver
    obj->drvVarsHandle = (DRVICVARS_Handle)&drvicVars_M1;
    obj->motorSetsHandle = (MOTORSETS_Handle)&motorSetVars_M1;
    obj->userParamsHandle = (userParams_Handle)&userParams_M1;
    obj->halMtrHandle = (HAL_MTR_Handle)(HAL_MTR1_init((void *)&halMtr_M1, sizeof(halMtr_M1)));
    return;
}

// initialize control parameters for motor 1
void initMotor1CtrlParameters(MOTOR_Handle handle)
{
    MOTOR_Vars_t *obj = (MOTOR_Vars_t *)handle;
    MOTOR_SetVars_t *objSets = (MOTOR_SetVars_t *)(handle->motorSetsHandle);
    USER_Params *objUser = (USER_Params *)(handle->userParamsHandle);

    // set the driver parameters
    HAL_MTR_setParams(obj->halMtrHandle, obj->userParamsHandle);

    // initialize the user parameters
    USER_setMotor1Params(obj->userParamsHandle);

    // set the control mode
	motorHandle_M1->flagEnableShowMode = false;
	motorHandle_M1->flagEnableOffsetCalc = false;
	motorHandle_M1->flagEnableAlignment = false;
	motorHandle_M1->brakingMode = DAMPING_BRAKE_MODE;
	motorHandle_M1->operateMode = OPERATE_MODE_IDLE;
	motorHandle_M1->uCtrlState = UCTRL_COARSE;
	motorHandle_M1->enablePosCtrl = false;
	motorHandle_M1->enableSpeedCtrl = false;
	motorHandle_M1->enableCurrentCtrl = true;
	motorHandle_M1->enableOpenLoopCtrl = false;

    obj->alignTimeDelay = (uint16_t)(objUser->ctrlFreq_Hz * 0.1f);          // 0.1s

    //************** Fault Monitor **************//
    objSets->lostPhaseSet_A = USER_M1_LOST_PHASE_CURRENT_A;
    objSets->unbalanceRatioSet = USER_M1_UNBALANCE_RATIO;
    objSets->overLoadSet_W = USER_M1_OVER_LOAD_POWER_W;
    objSets->stallCurrentSet_A = USER_M1_STALL_CURRENT_A;

    objSets->IsFailedChekSet_A = USER_M1_FAULT_CHECK_CURRENT_A;

    objSets->speedFailMaxSet_Hz = USER_M1_FAIL_SPEED_MAX_HZ;
    objSets->speedFailMinSet_Hz = USER_M1_FAIL_SPEED_MIN_HZ;

    objSets->toqueFailMinSet_Nm = USER_M1_TORQUE_FAILED_SET;

    objSets->maxPeakCurrent_A = USER_M1_ADC_FULL_SCALE_CURRENT_A * 0.495f;
    objSets->overCurrent_A = USER_MOTOR1_OVER_CURRENT_A;
    objSets->currentInv_sf = USER_M1_CURRENT_INV_SF;

    objSets->overVoltageFault_V = USER_M1_OVER_VOLTAGE_FAULT_V;
    objSets->overVoltageNorm_V = USER_M1_OVER_VOLTAGE_NORM_V;
    objSets->underVoltageFault_V = USER_M1_UNDER_VOLTAGE_FAULT_V;
    objSets->underVoltageNorm_V = USER_M1_UNDER_VOLTAGE_NORM_V;

    objSets->voltageFaultTimeSet = USER_M1_VOLTAGE_FAULT_TIME_SET;

    objSets->overLoadTimeSet = USER_M1_OVER_LOAD_TIME_SET;
    objSets->motorStallTimeSet = USER_M1_STALL_TIME_SET;
    objSets->unbalanceTimeSet = USER_M1_UNBALANCE_TIME_SET;
    objSets->lostPhaseTimeSet = USER_M1_LOST_PHASE_TIME_SET;
    objSets->overSpeedTimeSet = USER_M1_OVER_SPEED_TIME_SET;
    objSets->startupFailTimeSet = USER_M1_STARTUP_FAIL_TIME_SET;

    objSets->overCurrentTimesSet = USER_M1_OVER_CURRENT_TIMES_SET;

    obj->stopWaitTimeCnt = 0;
    objSets->stopWaitTimeSet = USER_M1_STOP_WAIT_TIME_SET;
    objSets->restartWaitTimeSet = USER_M1_RESTART_WAIT_TIME_SET;
    objSets->restartTimesSet = USER_M1_START_TIMES_SET;

    objSets->dacCMPValH = 2048U + 1024U;    // set default positive peak value
    objSets->dacCMPValL = 2048U - 1024U;    // set default negative peak value

    obj->adcData.voltage_sf = objUser->voltage_sf;
    obj->adcData.dcBusvoltage_sf = objUser->voltage_sf;

    obj->adcData.current_sf = objUser->current_sf;
    obj->flagEnableRestart = false;

    obj->faultMtrMask.all = MTR1_FAULT_MASK_SET;

    //************** Speed Control **************//
    obj->encLowSpdCompe = 0.17;
    obj->encLowSpdCompeUI = 5.0E-4;
    obj->posLowDeltaThreshold_rad = 0.1;
    obj->posLowTimeThreshold_rad = 0.1;
    obj->encFastSpdThreshold = USER_MOTOR1_ENC_FSPEED_WINDOW * objUser->ctrlFreq_Hz;
    obj->speedStart_Hz = USER_MOTOR1_SPEED_START_Hz;
    obj->speedForce_Hz = USER_MOTOR1_SPEED_FORCE_Hz;
    obj->speedFlyingStart_Hz = USER_MOTOR1_SPEED_FS_Hz;

    obj->accelerationMax_Hzps = USER_MOTOR1_ACCEL_MAX_Hzps;
    obj->accelerationStart_Hzps = USER_MOTOR1_ACCEL_START_Hzps;

    obj->VsRef_pu = 0.98f * USER_M1_MAX_VS_MAG_PU;
    obj->VsRef_V = 0.98f * USER_M1_MAX_VS_MAG_PU * USER_M1_NOMINAL_DC_BUS_VOLTAGE_V;

    obj->alignCurrent_A = USER_MOTOR1_ALIGN_CURRENT_A;
    obj->startCurrent_A = USER_MOTOR1_STARTUP_CURRENT_A;
    obj->maxCurrent_A = USER_MOTOR1_MAX_CURRENT_A;
    obj->IsSet_A = USER_MOTOR1_TORQUE_CURRENT_A;

    obj->power_sf = MATH_TWO_PI / USER_MOTOR1_NUM_POLE_PAIRS;
    obj->VIrmsIsrScale = objUser->ctrlFreq_Hz;

    obj->angleDelayed_sf = 0.5f * MATH_TWO_PI * USER_M1_CTRL_PERIOD_sec;

    //************** ABS Encoder **************//
    obj->encHandle = &encM1;

    //************** FOC Transform **************//

    // initialize the Clarke modules
    obj->clarkeHandle_V = &clarke_V_M1;

    // set the Clarke parameters
    setupClarke_V(obj->clarkeHandle_V, objUser->numVoltageSensors);

    // initialize the Clarke modules
    obj->clarkeHandle_I = &clarke_I_M1;

    // set the Clarke parameters
    setupClarke_I(obj->clarkeHandle_I, objUser->numCurrentSensors);

    // initialize the inverse Park module
    obj->iparkHandle_V = &ipark_V_M1;

    // initialize the Park module
    obj->parkHandle_I = &park_I_M1;

    // initialize the Park module
    obj->parkHandle_V = &park_V_M1;

    // initialize the PI controllers
    obj->piHandle_Id  = &pi_Id_M1;
    obj->piHandle_Iq  = &pi_Iq_M1;
    obj->piHandle_spd = &pi_spd_M1;
    obj->piHandle_pos = &pi_pos_M1;
//    obj->angleDeltaSumCount = 50;

    //************** SVPWM **************//

#if MTR1_SVGEN
    // initialize the space vector generator module
    obj->svgenHandle = &svgen_M1;

    if(objUser->flag_bypassMotorId == true)
    {

        obj->svmMode = SVM_MIN_C;
        obj->flagEnableFWC = true;
    }
    else
    {
        obj->svmMode = SVM_COM_C;
        obj->flagEnableFWC = false;
    }
#elif MTR1_SVPWM
    // initialize the space vector generator module
    obj->svpwmHandle = &svpwm_M1;

    obj->svpwmHandle->Ts = 0.0001;
    obj->svpwmHandle->period = 8000;
    obj->svpwmHandle->udc = 24.0f;
#endif

    HAL_setTriggerPrams(&obj->pwmData, USER_SYSTEM_FREQ_MHz, 0.2f, 0.1f);

    //************** TRAJ **************//

    // initialize the speed reference trajectory
    obj->trajHandle_spd = &traj_spd_M1;

    // configure the speed reference trajectory (Hz)
    TRAJ_setTargetValue(obj->trajHandle_spd, 0.0f);
    TRAJ_setIntValue(obj->trajHandle_spd, 0.0f);
    TRAJ_setMinValue(obj->trajHandle_spd, -objUser->maxFrequency_Hz);
    TRAJ_setMaxValue(obj->trajHandle_spd, objUser->maxFrequency_Hz);
    TRAJ_setMaxDelta(obj->trajHandle_spd, (objUser->maxAccel_Hzps / objUser->ctrlFreq_Hz));

#if MTR1_DRV8323RS

    //************** DRIVER DRV8323RS **************//

    // turn on the DRV8323/DRV8353/DRV8316 if present
    HAL_enableDRV(obj->halMtrHandle);

    // initialize the DRV8323/DRV8353/DRV8316 interface
    HAL_setupDRVSPI(obj->halMtrHandle, &drvicVars_M1);

    drvicVars_M1.ctrlReg02.bit.OTW_REP = true;
    drvicVars_M1.ctrlReg02.bit.PWM_MODE = DRV8323_PWMMODE_6;

    drvicVars_M1.ctrlReg05.bit.VDS_LVL = DRV8323_VDS_LEVEL_1P700_V;
    drvicVars_M1.ctrlReg05.bit.OCP_MODE = DRV8323_AUTOMATIC_RETRY;
    drvicVars_M1.ctrlReg05.bit.DEAD_TIME = DRV8323_DEADTIME_100_NS;
    drvicVars_M1.ctrlReg06.bit.CSA_GAIN = DRV8323_Gain_10VpV;

    drvicVars_M1.ctrlReg06.bit.LS_REF = false;
    drvicVars_M1.ctrlReg06.bit.VREF_DIV = true;
    drvicVars_M1.ctrlReg06.bit.CSA_FET = false;

    // disable the PWM
    HAL_disablePWM(obj->halMtrHandle);

    drvicVars_M1.writeCmd = 1;
    HAL_writeDRVData(obj->halMtrHandle, &drvicVars_M1);

    obj->uCtrlState = UCTRL_COARSE;
    obj->uENCState = UENC_IDLE;

    // setup the controllers, speed, d/q-axis current pid regulator
    setupControllers(handle);

    // setup faults
    HAL_setupMtrFaults(obj->halMtrHandle);

#endif

    // disable the PWM
    HAL_disablePWM(obj->halMtrHandle);

    return;
}   // end of initMotor1CtrlParameters() function

void runMotor1OffsetsCalculation(MOTOR_Handle handle)
{
    MOTOR_Vars_t *obj = (MOTOR_Vars_t *)handle;
    MOTOR_SetVars_t *objSets = (MOTOR_SetVars_t *)(handle->motorSetsHandle);

    // calculate motor and pfc protection value
    calcMotorOverCurrentThreshold(handle);

    HAL_setMtrCMPSSDACValue(obj->halMtrHandle, objSets->dacCMPValH, objSets->dacCMPValL);

    ADC_setPPBReferenceOffset(MTR1_IU_ADC_BASE, MTR1_IU_ADC_PPB_NUM,
    		(uint32_t)USER_M1_IA_OFFSET_AD);

    ADC_setPPBReferenceOffset(MTR1_IV_ADC_BASE, MTR1_IV_ADC_PPB_NUM,
    		(uint32_t)USER_M1_IB_OFFSET_AD);

    ADC_setPPBReferenceOffset(MTR1_IW_ADC_BASE, MTR1_IW_ADC_PPB_NUM,
    		(uint32_t)USER_M1_IC_OFFSET_AD);

    // Offsets in phase current sensing
    obj->adcData.offset_I_ad.value[0]  =  USER_M1_IA_OFFSET_AD ;
    obj->adcData.offset_I_ad.value[1]  =  USER_M1_IB_OFFSET_AD ;
    obj->adcData.offset_I_ad.value[2]  =  USER_M1_IC_OFFSET_AD ;

    // Offsets in phase voltage sensing
    obj->adcData.offset_V_sf.value[0]  = USER_M1_VA_OFFSET_SF;
    obj->adcData.offset_V_sf.value[1]  = USER_M1_VB_OFFSET_SF;
    obj->adcData.offset_V_sf.value[2]  = USER_M1_VC_OFFSET_SF;

    if(obj->flagEnableOffsetCalc == true)
    {
        float32_t offsetK1 = 0.998001f;  // Offset filter coefficient K1: 0.05/(T+0.05);
        float32_t offsetK2 = 0.001999f;  // Offset filter coefficient K2: T/(T+0.05);
        float32_t invCurrentSf = 1.0f / obj->adcData.current_sf;

        float32_t invVdcbus;

        uint16_t offsetCnt;

        SysCtl_delay(50U);

        ADC_setPPBReferenceOffset(MTR1_IU_ADC_BASE, MTR1_IU_ADC_PPB_NUM, 0);
        ADC_setPPBReferenceOffset(MTR1_IV_ADC_BASE, MTR1_IV_ADC_PPB_NUM, 0);
        ADC_setPPBReferenceOffset(MTR1_IW_ADC_BASE, MTR1_IW_ADC_PPB_NUM, 0);

        obj->adcData.offset_I_ad.value[0] =
                 obj->adcData.offset_I_ad.value[0] * obj->adcData.current_sf;
        obj->adcData.offset_I_ad.value[1] =
                 obj->adcData.offset_I_ad.value[1] * obj->adcData.current_sf;
        obj->adcData.offset_I_ad.value[2] =
                 obj->adcData.offset_I_ad.value[2] * obj->adcData.current_sf;

        // Set the 3-phase output PWMs to 50% duty cycle
        obj->pwmData.Vabc_pu.value[0] = 0.0f;
        obj->pwmData.Vabc_pu.value[1] = 0.0f;
        obj->pwmData.Vabc_pu.value[2] = 0.0f;

        // write the PWM compare values
        HAL_writePWMData(obj->halMtrHandle, &obj->pwmData);

        // enable the PWM
        HAL_enablePWM(obj->halMtrHandle);

        for(offsetCnt = 0; offsetCnt < 32000; offsetCnt++)
        {
            // clear the ADC interrupt flag
            ADC_clearInterruptStatus(MTR1_ADC_INT_BASE, MTR1_ADC_INT_NUM);

            while(ADC_getInterruptStatus(MTR1_ADC_INT_BASE, MTR1_ADC_INT_NUM) == false);

            HAL_readMtr1ADCData(&obj->adcData);

            if(offsetCnt >= 2000)       // Ignore the first 2000 times
            {
                // Offsets in phase current sensing

                obj->adcData.offset_I_ad.value[0] =
                        offsetK1 * obj->adcData.offset_I_ad.value[0] +
                        obj->adcData.I_A.value[0] * offsetK2;

                obj->adcData.offset_I_ad.value[1] =
                        offsetK1 * obj->adcData.offset_I_ad.value[1] +
                        obj->adcData.I_A.value[1] * offsetK2;

                obj->adcData.offset_I_ad.value[2] =
                        offsetK1 * obj->adcData.offset_I_ad.value[2] +
                        obj->adcData.I_A.value[2] * offsetK2;

                invVdcbus = 1.0f / obj->adcData.VdcBus_V;

                // Offsets in phase voltage sensing
                obj->adcData.offset_V_sf.value[0] =
                         offsetK1 * obj->adcData.offset_V_sf.value[0] +
                         (invVdcbus * obj->adcData.V_V.value[0]) * offsetK2;

                obj->adcData.offset_V_sf.value[1] =
                         offsetK1 * obj->adcData.offset_V_sf.value[1] +
                         (invVdcbus * obj->adcData.V_V.value[1]) * offsetK2;

                obj->adcData.offset_V_sf.value[2] =
                         offsetK1 * obj->adcData.offset_V_sf.value[2] +
                         (invVdcbus * obj->adcData.V_V.value[2]) * offsetK2;
            }
            else
            {
                // enable the PWM
                HAL_enablePWM(obj->halMtrHandle);
            }
        } // for()

        // disable the PWM
        HAL_disablePWM(obj->halMtrHandle);

        obj->flagEnableOffsetCalc = false;

        obj->adcData.offset_I_ad.value[0] =
                 obj->adcData.offset_I_ad.value[0] * invCurrentSf;
        obj->adcData.offset_I_ad.value[1] =
                 obj->adcData.offset_I_ad.value[1] * invCurrentSf;
        obj->adcData.offset_I_ad.value[2] =
                 obj->adcData.offset_I_ad.value[2] * invCurrentSf;

        ADC_setPPBReferenceOffset(MTR1_IU_ADC_BASE, MTR1_IU_ADC_PPB_NUM,
                                  (uint16_t)obj->adcData.offset_I_ad.value[0]);

        ADC_setPPBReferenceOffset(MTR1_IV_ADC_BASE, MTR1_IV_ADC_PPB_NUM,
                                  (uint16_t)obj->adcData.offset_I_ad.value[1]);

        ADC_setPPBReferenceOffset(MTR1_IW_ADC_BASE, MTR1_IW_ADC_PPB_NUM,
                                  (uint16_t)obj->adcData.offset_I_ad.value[2]);

    }   // flagEnableOffsetCalc = true

    // Check current and voltage offset
    if( (obj->adcData.offset_I_ad.value[0] > USER_M1_IA_OFFSET_AD_MAX) ||
        (obj->adcData.offset_I_ad.value[0] < USER_M1_IA_OFFSET_AD_MIN) )
    {
        obj->faultMtrNow.bit.currentOffset = 1;
    }

    if( (obj->adcData.offset_I_ad.value[1] > USER_M1_IB_OFFSET_AD_MAX) ||
        (obj->adcData.offset_I_ad.value[1] < USER_M1_IB_OFFSET_AD_MIN) )
    {
        obj->faultMtrNow.bit.currentOffset = 1;
    }

    if( (obj->adcData.offset_I_ad.value[2] > USER_M1_IC_OFFSET_AD_MAX) ||
        (obj->adcData.offset_I_ad.value[2] < USER_M1_IC_OFFSET_AD_MIN) )
    {
        obj->faultMtrNow.bit.currentOffset = 1;
    }

    if( (obj->adcData.offset_V_sf.value[0] > USER_M1_VA_OFFSET_SF_MAX) ||
        (obj->adcData.offset_V_sf.value[0] < USER_M1_VA_OFFSET_SF_MIN) )
    {
        obj->faultMtrNow.bit.voltageOffset = 1;
    }

    if( (obj->adcData.offset_V_sf.value[1] > USER_M1_VB_OFFSET_SF_MAX) ||
        (obj->adcData.offset_V_sf.value[1] < USER_M1_VB_OFFSET_SF_MIN) )
    {
        obj->faultMtrNow.bit.voltageOffset = 1;
    }

    if( (obj->adcData.offset_V_sf.value[2] > USER_M1_VC_OFFSET_SF_MAX) ||
        (obj->adcData.offset_V_sf.value[2] < USER_M1_VC_OFFSET_SF_MIN) )
    {
        obj->faultMtrNow.bit.voltageOffset = 1;
    }
    return;
} // end of runMotor1OffsetsCalculation() function

void runMotor1Control(MOTOR_Handle handle)
{
    MOTOR_Vars_t *obj = (MOTOR_Vars_t *)handle;
    MOTOR_SetVars_t *objSets = (MOTOR_SetVars_t *)(handle->motorSetsHandle);
    USER_Params *objUser = (USER_Params *)(handle->userParamsHandle);

//    if(HAL_getPwmEnableStatus(obj->halMtrHandle) == true)
//    {
//        if(HAL_getMtrTripFaults(obj->halMtrHandle) != 0)
//        {
//            obj->faultMtrNow.bit.moduleOverCurrent = 1;
//        }
//    }

    obj->faultMtrPrev.all |= obj->faultMtrNow.all;
    obj->faultMtrUse.all = obj->faultMtrNow.all & obj->faultMtrMask.all;

//    HAL_setMtrCMPSSDACValue(obj->halMtrHandle, objSets->dacCMPValH, objSets->dacCMPValL);

    if(obj->flagEnableRunAndIdentify == true)
    {
        // Had some faults to stop the motor
        if(obj->faultMtrUse.all != 0)
        {
            if(obj->flagRunIdentAndOnLine == true)
            {
                obj->flagRunIdentAndOnLine = false;
                obj->mctrlState = MCTRL_FAULT_STOP;

                obj->stopWaitTimeCnt = objSets->restartWaitTimeSet;
                obj->restartTimesCnt++;

                if(obj->flagEnableRestart == false)
                {
                    obj->flagEnableRunAndIdentify = false;
                    obj->stopWaitTimeCnt = 0;
                }
            }
            else if(obj->stopWaitTimeCnt == 0)
            {
                if(obj->restartTimesCnt < objSets->restartTimesSet)
                {
                    obj->flagClearFaults = 1;
                }
                else
                {
                    obj->flagEnableRunAndIdentify = false;
                }
            }

            HAL_disablePWM(obj->halMtrHandle);
        }
        // Restart
        else if((obj->flagRunIdentAndOnLine == false) &&
                (obj->stopWaitTimeCnt == 0))
        {
            restartMotorControl(handle);
        }
    }
    // if(obj->flagEnableRunAndIdentify == false)
    else if(obj->flagRunIdentAndOnLine == true)
    {
		stopMotorControl(handle);
		obj->stopWaitTimeCnt = objSets->stopWaitTimeSet;
    }
    else
    {
    	/* do nothing */
    }

    if(obj->flagClearFaults == true)
    {
        HAL_clearMtrFaultStatus(obj->halMtrHandle);

        obj->faultMtrNow.all &= MTR_FAULT_CLEAR;
        obj->flagClearFaults = false;
    }

    if(obj->flagRunIdentAndOnLine == true)
    {
        if(HAL_getPwmEnableStatus(obj->halMtrHandle) == false)
        {
            HAL_enablePWM(obj->halMtrHandle);
        }

        if(obj->operateMode == OPERATE_MODE_SPEED)
        {
        	obj->speedRef_Hz = (float32_t)obj->speedRef_Rpm / 60.0f;
        }

		TRAJ_setTargetValue(obj->trajHandle_spd, obj->speedRef_Hz);

		if(fabsf(obj->speed_Hz) > obj->speedStart_Hz)
		{
			TRAJ_setMaxDelta(obj->trajHandle_spd,(obj->accelerationMax_Hzps / objUser->ctrlFreq_Hz));
//			PI_setMinMax(obj->piHandle_spd, -obj->maxCurrent_A, obj->maxCurrent_A);
		}
		else
		{
			TRAJ_setMaxDelta(obj->trajHandle_spd, (obj->accelerationStart_Hzps / objUser->ctrlFreq_Hz));

//			if(obj->speed_int_Hz >= 0.0f)
//			{
//				PI_setMinMax(obj->piHandle_spd, -obj->startCurrent_A, obj->startCurrent_A);
//			}
//			else
//			{
//				PI_setMinMax(obj->piHandle_spd, -obj->startCurrent_A, obj->startCurrent_A);
//			}
		}

    }
    else
    {
        // reset motor control parameters
        resetMotorControl(handle);
    }

    if(obj->flagSetupController == true)
    {
        // update the controller
        updateControllers(handle);
    }
    else
    {
        obj->flagSetupController = true;

        setupControllers(handle);
    }

    // update the global variables
    updateGlobalVariables(handle);

    return;
}   // end of the runMotor1Control() function

//
// xint3ISR - External Interrupt 3 ISR
//
__interrupt  void xint3ISR(void)
{
	encM1.isrCnt++;
	LIN_getData(LINA_BASE,encM1.receivedData);
}

// 16 0.0135713 // 32 0.0271426
#define DELTA_ISR_TIME	1.0E-4f		// s
#define DELTA_SAMPLE_TIME	0.01f	// s
#define SPEED_SAMPLE_COUNT (uint32_t)(DELTA_SAMPLE_TIME/DELTA_ISR_TIME)
uint32_t speedSampleCount = (uint32_t)(DELTA_SAMPLE_TIME/DELTA_ISR_TIME);
float32_t speedSampleTime = SPEED_SAMPLE_COUNT*DELTA_ISR_TIME;

#if MOTOR1_DEBUG
// DEBUG
#define MTR1_DEBUG_WINDOW	10 		// 0,0001 * 10 = 0.001s
float32_t debug1[200],debug2[200];
uint32_t dbCount = 0;
uint32_t dbLoop = 0;
#endif

//float32_t testRefIq = 0.05f;
volatile float32_t testFoc_rad = 0.0f;
volatile float32_t testDelta_rad = 0.003f;
volatile float32_t testAlignCurrent_A = 2.0f;
__interrupt CODE_SECTION( "ramfuncs") void motor1CtrlISR(void)
{
    MOTOR_Vars_t *obj = (MOTOR_Vars_t *)motorHandle_M1;
    USER_Params *objUser = (USER_Params *)(obj->userParamsHandle);
    MATH_Vec2 phasor;

    obj->ISRPeriodCount = HAL_readTimerCnt(halHandle,HAL_CPU_TIMER2);;
    obj->ISRPeriodCountDelta = obj->ISRPeriodCountPrev - obj->ISRPeriodCount;
    obj->ISRPeriodCountDelta_f = (float32_t)obj->ISRPeriodCountDelta/USER_SYSTEM_FREQ_Hz;
    obj->ISRPeriodCountPrev = obj->ISRPeriodCount;

    obj->ISRCount++;

    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP12);

#if MTR1_ABS_ENC
   // Gets the current mechanical angular position of the motor
   ENC_setThetaMechDeg(obj->encHandle);
   ENC_setThetaMechRad(obj->encHandle);

   obj->pos_rad = ENC_getThetaMechRad(obj->encHandle);
   obj->pos_rad = round(obj->pos_rad * 100000)/100000;

	// Calculate data for user visualization
	float32_t tempPosRef_deg,tempPosPointRef_deg,tempPosPoint_deg,tempPosPointRef_min,tempPosPoint_min;

   tempPosRef_deg = (float32_t)obj->posRef_rad*180.0f/MATH_PI;
   obj->posRef_deg = (uint32_t)tempPosRef_deg;
   obj->pos_deg = (uint32_t)ENC_getThetaMechDeg(obj->encHandle);

   tempPosPointRef_deg = (float32_t)tempPosRef_deg - (uint32_t)obj->posRef_deg;
   tempPosPoint_deg = (float32_t)ENC_getThetaMechDeg(obj->encHandle) - (uint32_t)obj->pos_deg;
   tempPosPointRef_min = (float32_t)(tempPosPointRef_deg * 60)-(uint32_t)(tempPosPointRef_deg * 60);
   tempPosPoint_min =(float32_t)(tempPosPoint_deg * 60)-(uint32_t)(tempPosPoint_deg * 60);

	obj->posRef_min = (uint32_t)(tempPosPointRef_deg * 60);
	obj->pos_min = (uint32_t)(tempPosPoint_deg * 60);

	obj->posRef_sec = (uint32_t)(tempPosPointRef_min * 60);
	obj->pos_sec = (uint32_t)(tempPosPoint_min * 60);

	// Determine whether the position loop tends to stabilize within each precision interval
	float32_t tempPosAcc_rad = fabsf(obj->posRef_rad-obj->pos_rad);
	(tempPosAcc_rad<(9.6962736E-6f*5))?(obj->posSTrimStableCount++):(obj->posSTrimStableCount=0); // 5deg
	(tempPosAcc_rad<(9.6962736E-6f*10))?(obj->posTrimStableCount++):(obj->posTrimStableCount=0); // 10deg

	(obj->posSTrimStableCount>=2000)?(obj->flagPosSTrimStable = 1):(obj->flagPosSTrimStable = 0);// warning
	(obj->posTrimStableCount>=2000)?(obj->flagPosTrimStable = 1):(obj->flagPosTrimStable = 0);// warning


	//************** Speed calculation **************//
    // 1. 高速路径: 固定窗口M/T法
	obj->encFastSpdCnt++;
	if(obj->encFastSpdCnt>=obj->encFastSpdThreshold)// 1000/10000=0.1s
	{
		obj->encFastSpdCnt=0;
		obj->posFastDelta_rad = obj->pos_rad - obj->posFastPrev_rad;

		// 处理角度回绕
		obj->posFastDelta_rad = MATH_incrAngle(obj->posFastDelta_rad, 0);

	    obj->speedENCFast_Hz = (obj->posFastDelta_rad / (obj->ISRPeriodCountDelta_f * obj->encFastSpdThreshold)) / MATH_TWO_PI;

		obj->posFastPrev_rad = obj->pos_rad;
	}

    // 速度融合与自适应调节
    float abs_omega_fast = fabsf(obj->speedENCFast_Hz);

    // 动态调整观测窗口
    if (abs_omega_fast > 10.0f) { // >10rad/s (约1.4Hz机械)
    	obj->encFastSpdThreshold = 100;
    }else {
    	obj->encFastSpdThreshold = 10;    // 20个周期(1ms)
    }

	// 2. 低速路径: 自适应角度累积法
    if (obj->encLowSpdCnt == 0) {
        obj->posLowDelta_rad = 0;
    }
    // 累积角度变化
    else {
    	obj->posLowDelta_rad = obj->pos_rad - obj->posLowPrev_rad;

        // 处理角度回绕
		obj->posLowDelta_rad = MATH_incrAngle(obj->posLowDelta_rad, 0);

        obj->posLowDeltaAccum_rad += obj->posLowDelta_rad;
    }
    obj->encLowSpdCnt++;
    obj->posLowPrev_rad = obj->pos_rad;

    // 低速路径更新条件：角度变化达到阈值或超时
    obj->posLowTimeAccum_s += obj->ISRPeriodCountDelta_f;
    // 1 sec = 4.8481368110953599358991410235795e-6 rad
    if (fabsf(obj->posLowDeltaAccum_rad) > obj->posLowDeltaThreshold_rad || obj->posLowTimeAccum_s > obj->posLowTimeThreshold_rad)// 0.1rad阈值或1000ms超时
    {
        if (obj->posLowTimeAccum_s > objUser->ctrlPeriod_sec) { // 避免除以0
            obj->speedENCSlow_Hz = obj->posLowDeltaAccum_rad / obj->posLowTimeAccum_s / MATH_TWO_PI;
        }

        obj->posLowDeltaAccum_rad = 0;
        obj->posLowTimeAccum_s = 0;
        obj->encLowSpdCnt = 0;
    }

    // 自适应融合系数 (基于速度大小)
    // 1. 计算融合权重
    float blend_factor = fminf(fmaxf(abs_omega_fast / 2.0f, 0.01f), 0.99f);

    // 2. 融合高速和低速路径
//    obj->speedENC_Hz = blend_factor * obj->speedENCFast_Hz + (1 - blend_factor) * obj->speedENCSlow_Hz;
    obj->speedENC_Hz = obj->speedENCFast_Hz;
//    obj->speedENC_Hz = obj->speedENCSlow_Hz;
#endif

	// Motor speed calculation results
	obj->speed_Hz = -obj->speedENC_Hz;
    obj->speedFilter_Hz = obj->speedFilter_Hz *0.8f + obj->speed_Hz * 0.2f;
    obj->speedAbs_Hz = fabsf(obj->speedFilter_Hz);

//	obj->speedRef_Rpm = (float32_t)(obj->speedRef_Hz * 60.0f);
	obj->speed_Rpm = (float32_t)(obj->speed_Hz * 60.0f);


    // Read the ADC data with offsets
    HAL_readMtr1ADCData(&obj->adcData);

    // Remove offsets
    obj->adcData.I_A.value[0] -= obj->adcData.offset_I_ad.value[0] * obj->adcData.current_sf;
    obj->adcData.I_A.value[1] -= obj->adcData.offset_I_ad.value[1] * obj->adcData.current_sf;
    obj->adcData.I_A.value[2] -= obj->adcData.offset_I_ad.value[2] * obj->adcData.current_sf;

    obj->adcData.I_A.value[0] = -obj->adcData.I_A.value[0];
    obj->adcData.I_A.value[1] = -obj->adcData.I_A.value[1];
    obj->adcData.I_A.value[2] = -obj->adcData.I_A.value[2];

    obj->adcData.V_V.value[0] -= obj->adcData.offset_V_sf.value[0] * obj->adcData.VdcBus_V;
    obj->adcData.V_V.value[1] -= obj->adcData.offset_V_sf.value[1] * obj->adcData.VdcBus_V;
    obj->adcData.V_V.value[2] -= obj->adcData.offset_V_sf.value[2] * obj->adcData.VdcBus_V;

    // run Clarke transform on voltage
    CLARKE_run(obj->clarkeHandle_V, &obj->adcData.V_V, &obj->Vab_V);

    // run Clarke transform on current
    CLARKE_run(obj->clarkeHandle_I, &obj->adcData.I_A, &obj->Iab_A);

	obj->oneOverDcBus_invV = 1.0f/obj->adcData.VdcBus_V;

	if(obj->flagRunIdentAndOnLine == true)
	{
		if(obj->speedRef_Hz > 0.0f)
		{
			obj->directionRef = 1.0f;
		}
		else
		{
			obj->directionRef = -1.0f;
		}

		// Operate mode select
		if(obj->enableOpenLoopCtrl)
		{
			obj->motorState = MOTOR_OL_CTRL;
			obj->operateMode = OPERATE_MODE_IDLE;
			obj->flagEnableShowMode = false;
			obj->enablePosCtrl = false;
			obj->enableSpeedCtrl = false;
			obj->enableCurrentCtrl = false;
		}
		if(obj->flagEnableShowMode)
		{
			obj->operateMode = OPERATE_MODE_POS;
			obj->enablePosCtrl = true;
			obj->enableSpeedCtrl = true;
			obj->enableCurrentCtrl = true;
		}
		else if(obj->enablePosCtrl)
		{
			obj->operateMode = OPERATE_MODE_POS;
			obj->enableSpeedCtrl = true;
			obj->enableCurrentCtrl = true;
		}
		else if(obj->enableSpeedCtrl)
		{
			obj->operateMode = OPERATE_MODE_SPEED;
			obj->flagEnableShowMode = false;
			obj->enableCurrentCtrl = true;
		}
		else if(obj->enableCurrentCtrl)
		{
			obj->operateMode = OPERATE_MODE_TORQUE;
			obj->flagEnableShowMode = false;
		}

	    if(obj->flagEnableBraking == true)
	    {
	        if(obj->motorState != MOTOR_BRAKE_STOP)
	        {
	            obj->motorState = MOTOR_BRAKE_STOP;

	            if((obj->brakingMode == HARDSWITCH_BRAKE_MODE)||
	            		obj->brakingMode == DAMPING_BRAKE_MODE)
	            {
	                // enable the braking mode PWM with
	                // turning-on three low side, turn off three high side
	                HAL_enableBrakePWM(obj->halMtrHandle);
	            }
	//            else if(obj->brakingMode == FORCESTOP_BRAKE_MODE)
	//            {
	//                obj->angleBrake_rad = obj->angleFOC_rad;
	//                PI_setRefValue(obj->piHandle_spd, 0.0f);
	//                PI_setUi(obj->piHandle_spd, 0.0f);
	//            }
	        }

			obj->operateMode = OPERATE_MODE_IDLE;
			obj->flagEnableShowMode = false;
			obj->enablePosCtrl = false;
			obj->enableSpeedCtrl = false;
			obj->enableCurrentCtrl = false;
			obj->enableSpeedCtrl = false;
			obj->enableCurrentCtrl = false;

	//        if(obj->brakingMode == FORCESTOP_BRAKE_MODE)
	//        {
	//            obj->IsRef_A = obj->brakingCurrent_A;
	//            obj->Idq_out_A.value[1] = obj->brakingCurrent_A;
	//
	//			obj->motorState = MOTOR_STOP_IDLE;
	//			obj->operateMode = OPERATE_MODE_IDLE;
	//			obj->flagEnableShowMode = false;
	//			obj->enablePosCtrl = false;
	//			obj->enableSpeedCtrl = false;
	//			obj->enableCurrentCtrl = false;
	//            obj->enableSpeedCtrl = false;
	//            obj->enableCurrentCtrl = true;
	//        }
	//        else
	//        {
	//			obj->motorState = MOTOR_STOP_IDLE;
	//			obj->operateMode = OPERATE_MODE_IDLE;
	//			obj->flagEnableShowMode = false;
	//			obj->enablePosCtrl = false;
	//			obj->enableSpeedCtrl = false;
	//			obj->enableCurrentCtrl = false;
	//            obj->enableSpeedCtrl = false;
	//            obj->enableCurrentCtrl = false;
	//        }
	    }

		// run a trajectory for speed reference,
		// so the reference changes with a ramp instead of a step
		obj->counterTrajSpeed++;
		if(obj->counterTrajSpeed >= objUser->numIsrTicksPerTrajTick)
		{
			// clear counter
			obj->counterTrajSpeed = 0;

			// Trajectory speed run
			TRAJ_run(obj->trajHandle_spd);
		}

		obj->flagEnableCurrentCtrl = obj->enableCurrentCtrl;
	}
	else
	{
	    if(obj->brakingMode == DAMPING_BRAKE_MODE)
	    {
	    	obj->flagEnableBraking = false;

	        if(obj->motorState != MOTOR_BRAKE_STOP)
	        {
	            obj->motorState = MOTOR_BRAKE_STOP;

	            if((obj->brakingMode == HARDSWITCH_BRAKE_MODE)||
	            		obj->brakingMode == DAMPING_BRAKE_MODE)
	            {
	                // enable the braking mode PWM with
	                // turning-on three low side, turn off three high side
	                HAL_enableBrakePWM(obj->halMtrHandle);
	            }
	//            else if(obj->brakingMode == FORCESTOP_BRAKE_MODE)
	//            {
	//                obj->angleBrake_rad = obj->angleFOC_rad;
	//                PI_setRefValue(obj->piHandle_spd, 0.0f);
	//                PI_setUi(obj->piHandle_spd, 0.0f);
	//            }
	        }

	//        if(obj->brakingMode == FORCESTOP_BRAKE_MODE)
	//        {
	//            obj->IsRef_A = obj->brakingCurrent_A;
	//            obj->Idq_out_A.value[1] = obj->brakingCurrent_A;
	//
	//			obj->motorState = MOTOR_STOP_IDLE;
	//			obj->operateMode = OPERATE_MODE_IDLE;
	//			obj->flagEnableShowMode = false;
	//			obj->enablePosCtrl = false;
	//			obj->enableSpeedCtrl = false;
	//			obj->enableCurrentCtrl = false;
	//            obj->enableSpeedCtrl = false;
	//            obj->enableCurrentCtrl = true;
	//        }
	//        else
	//        {
	//			obj->motorState = MOTOR_STOP_IDLE;
	//			obj->operateMode = OPERATE_MODE_IDLE;
	//			obj->flagEnableShowMode = false;
	//			obj->enablePosCtrl = false;
	//			obj->enableSpeedCtrl = false;
	//			obj->enableCurrentCtrl = false;
	//            obj->enableSpeedCtrl = false;
	//            obj->enableCurrentCtrl = false;
	//        }
	    }
	    else
	    {
			obj->flagEnableCurrentCtrl = false;
			obj->motorState = MOTOR_STOP_IDLE;
	    }
	}

    // Running state
	switch(obj->motorState)
	{
		case MOTOR_ALIGNMENT:
			obj->stateRunTimeCnt++;

			TRAJ_setIntValue(obj->trajHandle_spd, 0.0f);
			obj->IsRef_A = 0.0f;
	        obj->angleFOC_rad = 0.0f;

	        if((obj->flagEnableAlignment == true)&&(obj->stateRunTimeCnt > obj->alignTimeDelay))
	        {
				obj->Vdq_out_V.value[0] = testAlignCurrent_A;
		        obj->Vdq_out_V.value[1] = 0.0f;
		        // GL40
//		        obj->angleENCOffset_rad = MATH_incrAngle1(obj->pos_machine*6*USER_MOTOR1_NUM_POLE_PAIRS*(MATH_PI/180.0f), 0);
		        obj->angleENCOffset_rad = MATH_incrAngle1((MATH_TWO_PI-obj->pos_rad)*6*USER_MOTOR1_NUM_POLE_PAIRS, 0);

		        // GL35
//	        	obj->angleENCOffset_rad = MATH_incrAngle1((360.0f-obj->pos_machine)*6*USER_MOTOR1_NUM_POLE_PAIRS*(MATH_PI/180.0f), 0);

	            obj->stateRunTimeCnt = 0;
	            PI_setUi(obj->piHandle_spd, 0.0);
	        }
	        else if(obj->flagEnableAlignment == false)
	        {
	        	// 1) gl35 3.1902337
	        	// 2) gl40 0.761096
	        	obj->angleENCOffset_rad = 3.0136247;
	        	obj->stateRunTimeCnt = 0;
	        	obj->motorState = MOTOR_CL_RUNNING;
	            obj->Idq_out_A.value[0] = 0.0f;
	            PI_setUi(obj->piHandle_spd, 0.0);
	        }
			break;
		case MOTOR_OL_CTRL:
			if(obj->enableOpenLoopCtrl)
			{
				obj->flagEnableShowMode = false;
		    	obj->enablePosCtrl = false;
		    	obj->enableSpeedCtrl = false;
		    	obj->enableCurrentCtrl = false;

				//************** Electrical Angle for open-loop control **************//
		    	// 0) angleFoc for running state
//		    	obj->angleFOC_rad = MATH_incrAngle1((360.0f-obj->pos_machine)*6*USER_MOTOR1_NUM_POLE_PAIRS*(MATH_PI/180.0f)- obj->angleENCOffset_rad + MATH_PI,0);
		    	obj->angleFOC_rad = MATH_incrAngle3((MATH_TWO_PI-obj->pos_rad)*6*USER_MOTOR1_NUM_POLE_PAIRS- obj->angleENCOffset_rad,0,USER_MOTOR1_NUM_POLE_PAIRS);
//		    	obj->angleDelta_rad = obj->angleFOC_rad - obj->angleFOCPrev_rad;

		    	// 1) open loop with encoder
		    	// GL40
//		    	testFoc_rad = MATH_incrAngle1((360.0f-obj->pos_machine)*6*USER_MOTOR1_NUM_POLE_PAIRS*(MATH_PI/180.0f)- obj->angleENCOffset_rad + MATH_PI,0);
		    	testFoc_rad = MATH_incrAngle1((MATH_TWO_PI-obj->pos_rad)*6*USER_MOTOR1_NUM_POLE_PAIRS- obj->angleENCOffset_rad,0);

		    	// GL35
//		    	testFoc_rad = MATH_incrAngle1((360.0f-obj->pos_machine)*6*USER_MOTOR1_NUM_POLE_PAIRS*(MATH_PI/180.0f)- obj->angleENCOffset_rad + MATH_PI,0);

		    	// 2) open loop with increase elecAngle
//				testFoc_rad += testDelta_rad;
//		        testFoc_rad = MATH_incrAngle1(testFoc_rad,0);
//		        obj->angleFOC_rad = testFoc_rad;


//				// 电角度防抖处理
//				if(obj->directionRef == 1.0)
//				{
//					if(((obj->angleDelta_rad >= 0) && (obj->angleDelta_rad <= MATH_PI))||
//							((obj->angleDelta_rad < 0)&& (obj->angleDelta_rad <= -MATH_PI)))
//					{
//						obj->angleDelta_rad = MATH_incrAngle1(obj->angleDelta_rad,0);
//					}
//				}
//				else if(obj->directionRef == -1.0)
//				{
//					if(((obj->angleDelta_rad <= 0) && (obj->angleDelta_rad >= -MATH_PI))
//							||((obj->angleDelta_rad > 0)&& (obj->angleDelta_rad >= MATH_PI)))
//					{
//						obj->angleDelta_rad = MATH_incrAngle2(obj->angleDelta_rad,0);
//					}
//				}

		        obj->IsRef_A = obj->IsSet_A;
				obj->Vdq_out_V.value[0] = 0.0f;
				obj->Vdq_out_V.value[1] = obj->IsRef_A;
			}
			else
			{
				obj->Vdq_out_V.value[0] = 0.0f;
				obj->Vdq_out_V.value[1] = 0.0f;

				obj->motorState = MOTOR_CL_RUNNING;
			}
			break;
		case MOTOR_CL_RUNNING:
			//************** Electrical Angle calculation **************//
			obj->angleFOC_rad = MATH_incrAngle3((MATH_TWO_PI-obj->pos_rad)*6*USER_MOTOR1_NUM_POLE_PAIRS- obj->angleENCOffset_rad,0,USER_MOTOR1_NUM_POLE_PAIRS);
//			obj->angleDelta_rad = obj->angleFOC_rad - obj->angleFOCPrev_rad;

//			// 电角度防抖处理
//			if(obj->directionRef == 1.0)
//			{
//				if(((obj->angleDelta_rad >= 0) && (obj->angleDelta_rad <= MATH_PI))||
//						((obj->angleDelta_rad < 0)&& (obj->angleDelta_rad <= -MATH_PI)))
//				{
//					obj->angleDelta_rad = MATH_incrAngle1(obj->angleDelta_rad,0);
//				}
//			}
//			else if(obj->directionRef == -1.0)
//			{
//				if(((obj->angleDelta_rad <= 0) && (obj->angleDelta_rad >= -MATH_PI))
//						||((obj->angleDelta_rad > 0)&& (obj->angleDelta_rad >= MATH_PI)))
//				{
//					obj->angleDelta_rad = MATH_incrAngle2(obj->angleDelta_rad,0);
//				}
//			}
//
//			obj->angleFOCPrev_rad = obj->angleFOC_rad;

		    //************** Speed calculation **************//
		    obj->speed_int_Hz = TRAJ_getIntValue(obj->trajHandle_spd);

		    //************** Motor control mode select **************//
		    switch(obj->operateMode)
		    {
		    	case OPERATE_MODE_POS:
		    		// run the position controller
		    		obj->counterPos++;
		            if(obj->counterPos >= objUser->numCtrlTicksPerPosTick)
		            {
		                obj->counterPos = 0;
		                PI_run(obj->piHandle_pos,obj->posRef_rad,obj->pos_rad,(float32_t *)&obj->speedRef_Hz);
		                obj->speedRef_Hz = -obj->speedRef_Hz;
		            }
		            // run the speed controller
		            obj->counterSpeed++;
	                if(obj->counterSpeed >= objUser->numCtrlTicksPerSpeedTick)
		            {
	                	obj->counterSpeed = 0;
	                	PI_run(obj->piHandle_spd,obj->speedRef_Hz,obj->speed_Hz,(float32_t *)&obj->IsRef_A);
		            }
		    		break;
		    	case OPERATE_MODE_SPEED:
//		    		if(obj->speed_Hz != obj->speedPrev_Hz)
//		    		{
			            // run the speed controller
			            obj->counterSpeed++;
		                if(obj->counterSpeed >= objUser->numCtrlTicksPerSpeedTick)
			            {
		                	obj->counterSpeed = 0;
//		                	PI_runSpeed(obj->piHandle_spd,obj->speed_int_Hz,obj->speed_Hz,(float32_t *)&obj->IsRef_A,
//		                			obj->directionRef, obj->encLowSpdCompeUI, obj->encLowSpdCompe);
			            	PI_run(obj->piHandle_spd,obj->speed_int_Hz,obj->speed_Hz,(float32_t *)&obj->IsRef_A);
			            }
//		    		}
//		    		obj->speedPrev_Hz = obj->speed_Hz;
		    		break;
		    	case OPERATE_MODE_TORQUE:
		            if(obj->speed_int_Hz >= 0.0f)
		            {
		                obj->IsRef_A = obj->IsSet_A;
		            }
		            else
		            {
		                obj->IsRef_A = -obj->IsSet_A;
		            }
		            // for switching back speed closed-loop control
		            PI_setUi(obj->piHandle_spd, obj->IsRef_A);
		    		break;
		    	default:
//		    		stopMotorControl(obj);
		    		break;
		    }

    		break;
		default:
//			stopMotorControl(obj);
			break;
	}

	// compute the sin/cos phasor
	if(obj->enableOpenLoopCtrl)
	{
		phasor.value[0] = __cos(testFoc_rad);
		phasor.value[1] = __sin(testFoc_rad);
	}
	else
	{
		phasor.value[0] = __cos(obj->angleFOC_rad);
		phasor.value[1] = __sin(obj->angleFOC_rad);
	}

    // set the phasor in the Park transform
    PARK_setPhasor(obj->parkHandle_I, &phasor);

    // run the Park transform
    PARK_run(obj->parkHandle_I, &(obj->Iab_A),(MATH_vec2 *)&(obj->Idq_in_A));

    if(obj->flagEnableCurrentCtrl)
    {
    	if(obj->operateMode == OPERATE_MODE_SPEED)
    	{
    		obj->Idq_out_A.value[0] = obj->IdRef_A;
    	}
    	else
    	{
    		obj->Idq_out_A.value[0] = 0.0f;
    	}
        obj->Idq_out_A.value[1] = obj->IsRef_A;

//        obj->IdqRef_A.value[0] = obj->Idq_out_A.value[0] + obj->IdRated_A;
        obj->IdqRef_A.value[0] = obj->Idq_out_A.value[0];
        obj->IdqRef_A.value[1] = obj->Idq_out_A.value[1];

        // Maximum voltage output
        objUser->maxVsMag_V = objUser->maxVsMag_pu * obj->adcData.VdcBus_V;

        PI_setMinMax(obj->piHandle_Id,-objUser->maxVsMag_V, objUser->maxVsMag_V);
        PI_run(obj->piHandle_Id, obj->IdqRef_A.value[0],obj->Idq_in_A.value[0], (float32_t*)&obj->Vdq_out_V.value[0]);

        // calculate Iq controller limits
        float32_t outMax_V = __sqrt((objUser->maxVsMag_V * objUser->maxVsMag_V) - (obj->Vdq_out_V.value[0] * obj->Vdq_out_V.value[0]));

        PI_setMinMax(obj->piHandle_Iq, -outMax_V, outMax_V);
        PI_run(obj->piHandle_Iq, obj->IdqRef_A.value[1], obj->Idq_in_A.value[1], (float32_t*)&obj->Vdq_out_V.value[1]);
    }

    // set the phasor in the inverse Park transform
    IPARK_setPhasor(obj->iparkHandle_V, &phasor);

    // run the inverse Park module
    IPARK_run(obj->iparkHandle_V, &obj->Vdq_out_V, &obj->Vab_out_V);

#if MTR1_SV_MODE == 5
    // setup the space vector generator (SVGEN) module
    SVGEN_setup(obj->svgenHandle, obj->oneOverDcBus_invV);

    // run the space vector generator (SVGEN) module
    SVGEN_run(obj->svgenHandle, &obj->Vab_out_V, &(obj->pwmData.Vabc_pu));
#endif
#if MTR1_SV_MODE == 7
    // setup the space vector generator (SVGEN) module
//    SVGEN_setup(obj->svgenHandle, obj->oneOverDcBus_invV);

    obj->svgenHandle->Ts = obj->ISRPeriodCountDelta_f;
    obj->svgenHandle->udc = obj->adcData.VdcBus_V;
    // run the space vector generator (SVGEN) module
    SVPWM_run(obj->svgenHandle, &obj->Vab_out_V, &(obj->pwmData.Vabc_pu));
#endif

    if(HAL_getPwmEnableStatus(obj->halMtrHandle) == false)
    {
        // clear PWM data
        obj->pwmData.Vabc_pu.value[0] = 0.0f;
        obj->pwmData.Vabc_pu.value[1] = 0.0f;
        obj->pwmData.Vabc_pu.value[2] = 0.0f;
    }

    // write the PWM compare values
    HAL_writePWMData(obj->halMtrHandle, &obj->pwmData);

    // acknowledge the ADC interrupt
    HAL_ackMtr1ADCInt();

    // Collect current and voltage data to calculate the RMS value
    collectRMSData(motorHandle_M1);

//    obj->ISRPeriodCountPrev = HAL_readTimerCnt(halHandle,HAL_CPU_TIMER2);;
//    obj->ISRPeriodCountDelta = obj->ISRPeriodCount - obj->ISRPeriodCountPrev;
//    obj->ISRPeriodCountDelta_f = (float32_t)obj->ISRPeriodCountDelta/USER_SYSTEM_FREQ_Hz;
//
//    if(obj->ISRPeriodCountDelta_f>9.65625E-6)
//    {
//    	// code
//    }


#if MOTOR1_DEBUG
    dbLoop++;

    // window = 2ms
    if(dbLoop >= 5)
    {
        if(dbCount>=200)
        {
        	dbCount = 0;
        }

        debug1[dbCount] = obj->adcData.I_A.value[0];
//        debug2[dbCount] = obj->pos_rad;

        dbCount++;
        dbLoop = 0;
    }
#endif
    return;
} // end of motor1CtrlISR() function

