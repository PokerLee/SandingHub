#include "sys_main.h"
#include "motor1_drive.h"
#include "always.h"

CODE_SECTION(".data")  	volatile  	MOTOR_Handle  	motorHandle_M1;
CODE_SECTION(".data")  	volatile 	MOTOR_Vars_t 	motorVars_M1;

CODE_SECTION(".data") 	USER_Params 	userParams_M1;
CODE_SECTION(".data")  	MOTOR_SetVars_t motorSetVars_M1;
CODE_SECTION(".data") 	HAL_MTR_Obj    	halMtr_M1;
CODE_SECTION(".data")  	DRVIC_VARS_t 	drvicVars_M1;
CODE_SECTION(".data")   CLARKE_Obj    	clarke_V_M1;
CODE_SECTION(".data")  	CLARKE_Obj    	clarke_I_M1;
CODE_SECTION(".data")  	IPARK_Obj     	ipark_V_M1;
CODE_SECTION(".data") 	PARK_Obj      	park_I_M1;
CODE_SECTION(".data") 	PARK_Obj      	park_V_M1;
CODE_SECTION(".data")  	PI_Obj        	pi_Id_M1;
CODE_SECTION(".data")  	PI_Obj        	pi_Iq_M1;
CODE_SECTION(".data")   PI_Obj        	pi_spd_M1;
CODE_SECTION(".data")   PI_Obj        	pi_pos_M1;
CODE_SECTION(".data") 	SVGEN_Obj     	svgen_M1;
CODE_SECTION(".data") 	TRAJ_Obj     	traj_spd_M1;

void initMotor1Handles(MOTOR_Handle handle)
{
    MOTOR_Vars_t *obj = (MOTOR_Vars_t *)handle;

    // initialize the driver
    obj->halMtrHandle = HAL_MTR1_init(&halMtr_M1, sizeof(halMtr_M1));
    obj->drvVarsHandle = &drvicVars_M1;

    obj->motorSetsHandle = &motorSetVars_M1;
    obj->userParamsHandle = &userParams_M1;
    obj->estHandle = (EST_Handle)est_motor_1;
    return;
}

// initialize control parameters for motor 1
void initMotor1CtrlParameters(MOTOR_Handle handle)
{
    MOTOR_Vars_t *obj = (MOTOR_Vars_t *)handle;
    MOTOR_SetVars_t *objSets = (MOTOR_SetVars_t *)(handle->motorSetsHandle);
    USER_Params *objUser = (USER_Params *)(handle->userParamsHandle);

    obj->flagEnableForceAngle = true;
    obj->flagEnableFlyingStart = false;
    obj->flagEnableIPD = false;

    obj->flagEnableSpeedCtrl = true;
    obj->flagEnableCurrentCtrl = true;
    obj->flagEnableAlignment = true;
    obj->alignTimeDelay = (uint16_t)(objUser->ctrlFreq_Hz * 0.1f);          // 0.1s
    obj->forceRunTimeDelay = (uint16_t)(objUser->ctrlFreq_Hz * 1.0f);       // 1.0s

    obj->fwcTimeDelay = (uint16_t)(objUser->ctrlFreq_Hz * 2.0f);        // 2.0s

    // set the driver parameters
    HAL_MTR_setParams(obj->halMtrHandle, obj->userParamsHandle);

//    objSets->Kp_fwc = USER_M1_FWC_KP;
//    objSets->Ki_fwc = USER_M1_FWC_KI;
//    objSets->angleFWCMax_rad = USER_M1_FWC_MAX_ANGLE_RAD;
//    objSets->overModulation = USER_M1_MAX_VS_MAG_PU;

    //************** FAST Estimator **************//

    objSets->RsOnLineCurrent_A = 0.1f * USER_MOTOR1_MAX_CURRENT_A;
    obj->estState = EST_STATE_IDLE;
    obj->trajState = EST_TRAJ_STATE_IDLE;

    // for Rs online calibration
    obj->flagRsOnLineContinue = false;
    obj->flagStartRsOnLine = false;

    objSets->RsOnlineWaitTimeSet = USER_MOTOR1_RSONLINE_WAIT_TIME;
    objSets->RsOnlineWorkTimeSet = USER_MOTOR1_RSONLINE_WORK_TIME;

    // initialize the user parameters
    USER_setParams_priv(obj->userParamsHandle);

    // initialize the user parameters
    USER_setMotor1Params(obj->userParamsHandle);

    // set the default estimator parameters
    EST_setParams(obj->estHandle, obj->userParamsHandle);
    EST_setFlag_enableForceAngle(obj->estHandle,obj->flagEnableForceAngle);
    EST_setFlag_enableRsRecalc(obj->estHandle,obj->flagEnableRsRecalc);
    EST_setOneOverFluxGain_sf(obj->estHandle, obj->userParamsHandle, USER_M1_EST_FLUX_HF_SF);
    EST_setFreqLFP_sf(obj->estHandle, obj->userParamsHandle, USER_M1_EST_FREQ_HF_SF);
    EST_setBemf_sf(obj->estHandle, obj->userParamsHandle, USER_M1_EST_BEMF_HF_SF);

    // if motor is an induction motor, configure default state of PowerWarp
    if(objUser->motor_type == MOTOR_TYPE_INDUCTION)
    {
        EST_setFlag_enablePowerWarp(obj->estHandle, obj->flagEnablePowerWarp);
        EST_setFlag_bypassLockRotor(obj->estHandle, obj->flagBypassLockRotor);
    }

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

    obj->operateMode = OPERATE_MODE_SPEED;
    obj->faultMtrMask.all = MTR1_FAULT_MASK_SET;

    obj->speedStart_Hz = USER_MOTOR1_SPEED_START_Hz;
    obj->speedForce_Hz = USER_MOTOR1_SPEED_FORCE_Hz;
    obj->speedFlyingStart_Hz = USER_MOTOR1_SPEED_FS_Hz;

    obj->accelerationMax_Hzps = USER_MOTOR1_ACCEL_MAX_Hzps;
    obj->accelerationStart_Hzps = USER_MOTOR1_ACCEL_START_Hzps;

    obj->VsRef_pu = 0.98f * USER_M1_MAX_VS_MAG_PU;
    obj->VsRef_V = 0.98f * USER_M1_MAX_VS_MAG_PU * USER_M1_NOMINAL_DC_BUS_VOLTAGE_V;

    obj->fluxCurrent_A = USER_MOTOR1_FLUX_CURRENT_A;
    obj->alignCurrent_A = USER_MOTOR1_ALIGN_CURRENT_A;
    obj->startCurrent_A = USER_MOTOR1_STARTUP_CURRENT_A;
    obj->maxCurrent_A = USER_MOTOR1_MAX_CURRENT_A;
    obj->IsSet_A = USER_MOTOR1_TORQUE_CURRENT_A;

    obj->power_sf = MATH_TWO_PI / USER_MOTOR1_NUM_POLE_PAIRS;
    obj->VIrmsIsrScale = objUser->ctrlFreq_Hz;

    obj->angleDelayed_sf = 0.5f * MATH_TWO_PI * USER_M1_CTRL_PERIOD_sec;

    obj->flyingStartTimeDelay = (uint16_t)(objUser->ctrlFreq_Hz * 0.5f); // 0.5s

    obj->flyingStartMode = FLYINGSTART_MODE_HALT;

//#if MTR1_SV_MODE == 5
//#if MTR1_ID
//    if(objUser->flag_bypassMotorId == true)
//    {
//
//        obj->svmMode = SVM_MIN_C;
//        obj->flagEnableFWC = true;
//    }
//    else
//    {
//        obj->svmMode = SVM_COM_C;
//        obj->flagEnableFWC = false;
//#else
//        obj->svmMode = SVM_COM_C;
//        obj->flagEnableFWC = false;
//#endif
//    }
//#endif

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

    // initialize the space vector generator module
    obj->svgenHandle = &svgen_M1;

    SVGEN_setMode(obj->svgenHandle, SVM_COM_C);

    HAL_setTriggerPrams(&obj->pwmData, USER_SYSTEM_FREQ_MHz, 0.2f, 0.1f);

    // initialize the speed reference trajectory
    obj->trajHandle_spd = &traj_spd_M1;

    // configure the speed reference trajectory (Hz)
    TRAJ_setTargetValue(obj->trajHandle_spd, 0.0f);
    TRAJ_setIntValue(obj->trajHandle_spd, 0.0f);
    TRAJ_setMinValue(obj->trajHandle_spd, -objUser->maxFrequency_Hz);
    TRAJ_setMaxValue(obj->trajHandle_spd, objUser->maxFrequency_Hz);
    TRAJ_setMaxDelta(obj->trajHandle_spd, (objUser->maxAccel_Hzps / objUser->ctrlFreq_Hz));

    //
    // DRV8323RS
    //

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

//    // disable the PWM
//    HAL_disablePWM(obj->halMtrHandle);
//
//    drvicVars_M1.writeCmd = 1;
//    HAL_writeDRVData(obj->halMtrHandle, &drvicVars_M1);

    // setup the controllers, speed, d/q-axis current pid regulator
    setupControllers(handle);

    // setup faults
//    HAL_setupMtrFaults(obj->halMtrHandle);

    // disable the PWM
    HAL_disablePWM(obj->halMtrHandle);

    return;
}   // end of initMotor1CtrlParameters() function

//
// Defines
//
#define ARR_LEN	5

//
// Globals
//
//float32_t mt_angle = 0.0f;
//float32_t el_angle = 0.0f;
float32_t el_angleOffset = 0.0f;
//float32_t machineAngle = 0.0f;

uint32_t xint1Count;
volatile uint8_t receivedChar[ARR_LEN];

void runMotor1OffsetsCalculation(MOTOR_Handle handle)
{
    MOTOR_Vars_t *obj = (MOTOR_Vars_t *)handle;
    MOTOR_SetVars_t *objSets = (MOTOR_SetVars_t *)(handle->motorSetsHandle);

    // calculate motor and pfc protection value
    calcMotorOverCurrentThreshold(handle);

    HAL_setMtrCMPSSDACValue(obj->halMtrHandle,
                            objSets->dacCMPValH, objSets->dacCMPValL);


    ADC_setPPBReferenceOffset(MTR1_IU_ADC_BASE, MTR1_IU_ADC_PPB_NUM,
    		(uint32_t)USER_M1_IA_OFFSET_AD);

    ADC_setPPBReferenceOffset(MTR1_IV_ADC_BASE, MTR1_IV_ADC_PPB_NUM,
    		(uint32_t)USER_M1_IB_OFFSET_AD);

    ADC_setPPBReferenceOffset(MTR1_IW_ADC_BASE, MTR1_IW_ADC_PPB_NUM,
    		(uint32_t)USER_M1_IC_OFFSET_AD);

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

    HAL_setMtrCMPSSDACValue(obj->halMtrHandle, objSets->dacCMPValH, objSets->dacCMPValL);

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

            if(obj->flagEnableMotorIdentify == true)
            {
                obj->flagMotorIdentified = false;
                obj->flagSetupController = false;
//                obj->svmMode = SVM_COM_C;
//                obj->flagEnableFWC = false;
                obj->flagEnableMTPA = false;

                obj->speedRef_Hz = objUser->fluxExcFreq_Hz;

                objUser->flag_bypassMotorId = false;

                // disable interrupts
                DINT;
                __asm("  NOP");

                // set the default estimator parameters
                EST_setParams(obj->estHandle, obj->userParamsHandle);

                // enable interrupts
                EINT;
                __asm("  NOP");

                obj->flagEnableMotorIdentify = false;
            }

            restartMotorControl(handle);
        }
    }
    // if(obj->flagEnableRunAndIdentify == false)
    else if(obj->flagRunIdentAndOnLine == true)
    {
        stopMotorControl(handle);

        if(obj->flagEnableFlyingStart == false)
        {
            obj->stopWaitTimeCnt = objSets->stopWaitTimeSet;
        }
        else
        {
            obj->stopWaitTimeCnt = 0;
        }
    }
    else
    {
    }

    // enable or disable bypassLockRotor flag
    if(objUser->motor_type == MOTOR_TYPE_INDUCTION)
    {
        EST_setFlag_bypassLockRotor(obj->estHandle,obj->flagBypassLockRotor);
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
            // enable the estimator
            EST_enable(obj->estHandle);

            // enable the trajectory generator
            EST_enableTraj(obj->estHandle);

            // enable the PWM
            HAL_enablePWM(obj->halMtrHandle);
        }

        if(obj->flagMotorIdentified == true)
        {
            if(obj->speedRef_Hz > 0.0f)
            {
                obj->direction = 1.0f;
            }
            else
            {
                obj->direction = -1.0f;
            }

            // enable or disable force angle
            EST_setFlag_enableForceAngle(obj->estHandle, obj->flagEnableForceAngle);

            EST_setFlag_enableRsRecalc(obj->estHandle, obj->flagEnableRsRecalc);

            TRAJ_setTargetValue(obj->trajHandle_spd, obj->speedRef_Hz);

            if(fabsf(obj->speed_Hz) > obj->speedStart_Hz)
            {
                TRAJ_setMaxDelta(obj->trajHandle_spd,
                  (obj->accelerationMax_Hzps / objUser->ctrlFreq_Hz));

                PI_setMinMax(obj->piHandle_spd, -obj->maxCurrent_A, obj->maxCurrent_A);

//                SVGEN_setMode(obj->svgenHandle, obj->svmMode);

                if(obj->motorState >= MOTOR_CL_RUNNING)
                {
                    obj->stateRunTimeCnt++;

                    if(obj->stateRunTimeCnt == obj->fwcTimeDelay)
                    {
                        obj->Idq_out_A.value[0] = 0.0f;
                        obj->motorState = MOTOR_CTRL_RUN;
                        obj->mctrlState = MCTRL_CONT_RUN;
                    }
                }
            }
            else
            {
                TRAJ_setMaxDelta(obj->trajHandle_spd, (obj->accelerationStart_Hzps / objUser->ctrlFreq_Hz));

                if(obj->speed_int_Hz >= 0.0f)
                {
                    PI_setMinMax(obj->piHandle_spd, -obj->startCurrent_A, obj->startCurrent_A);
                }
                else
                {
                    PI_setMinMax(obj->piHandle_spd, -obj->startCurrent_A, obj->startCurrent_A);
                }

            }
        }

    }
    else
    {
        // reset motor control parameters
        resetMotorControl(handle);
    }

#if MTR1_ID
    // check the trajectory generator
    if(EST_isTrajError(obj->estHandle) == true)
    {
        // disable the PWM
        HAL_disablePWM(obj->halMtrHandle);
    }
    else
    {
        // update the trajectory generator state
        EST_updateTrajState(obj->estHandle);
    }

    // check the estimator
    if(EST_isError(obj->estHandle) == true)
    {
        // disable the PWM
        HAL_disablePWM(obj->halMtrHandle);
    }
    else        // No any estimator error
    {
        bool flagEstStateChanged = false;

        float32_t Id_target_A = EST_getIntValue_Id_A(obj->estHandle);

        if(obj->flagMotorIdentified == true)
        {
            flagEstStateChanged = EST_updateState(obj->estHandle, 0.0f);
        }
        else
        {
            flagEstStateChanged = EST_updateState(obj->estHandle, Id_target_A);
        }

        if(flagEstStateChanged == true)
        {
            // configure the trajectory generator
            EST_configureTraj(obj->estHandle);

            if(obj->flagMotorIdentified == false)
            {
                // configure the controllers
                EST_configureTrajState(obj->estHandle,
                					 	 obj->userParamsHandle,
                                         obj->piHandle_spd,
                                         obj->piHandle_Id,
										 obj->piHandle_Iq);
            }

            if(objUser->flag_bypassMotorId == false)
            {
                if((EST_isLockRotor(obj->estHandle) == true) ||
                		((EST_isMotorIdentified(obj->estHandle) == true)
                				&& (EST_isIdle(obj->estHandle) == true) ) )
                {
                    if(EST_isMotorIdentified(obj->estHandle) == true)
                    {
                        obj->flagMotorIdentified = true;

                        // clear the flag
                        obj->flagRunIdentAndOnLine = false;
                        obj->flagEnableRunAndIdentify = false;
                        objUser->flag_bypassMotorId = true;

                        // disable the estimator
                        EST_disable(obj->estHandle);

                        // enable the trajectory generator
                        EST_disableTraj(obj->estHandle);
                    }

                    if(objUser->motor_type == MOTOR_TYPE_INDUCTION)
                    {
                        // clear the flag
                        obj->flagRunIdentAndOnLine = false;
                        obj->flagEnableRunAndIdentify = false;
                    }
                }
            }
        }
    }

    obj->flagMotorIdentified = EST_isMotorIdentified(obj->estHandle);
    if(obj->flagMotorIdentified == true)
    {
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
    }
#else
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
#endif

    // run Rs online
    runRsOnLine(handle);

    // update the global variables
    updateGlobalVariables(handle);

//    // trigger XINT for absolute encoder
//    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP12);

    return;
}   // end of the runMotor1Control() function


//
// xint1ISR - External Interrupt 1 ISR
//
__interrupt  void xint3ISR(void)
{
    xint1Count++;

    uint8_t i = 0;
    for(i=0;i<ARR_LEN;i++)
    {
		receivedChar[i] = SCI_readCharBlockingFIFO(SCIA_BASE);
    }

//    // trigger XINT for absolute encoder
//    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP12);
}

// 16 0.0135713 // 32 0.0271426
#define DELTA_ISR_TIME	1.0E-4f		// s
#define DELTA_SAMPLE_TIME	0.01f	// s
#define SPEED_SAMPLE_COUNT (uint32_t)(DELTA_SAMPLE_TIME/DELTA_ISR_TIME)
uint32_t speedSampleCount = (uint32_t)(DELTA_SAMPLE_TIME/DELTA_ISR_TIME);
float32_t speedSampleTime = SPEED_SAMPLE_COUNT*DELTA_ISR_TIME;

// DEBUG
float32_t debug1[200],debug2[200],debug3[200];
uint32_t dbCount = 0;
uint32_t dbLoop = 10;
float32_t testRefIq = 0.3f;
float32_t testFoc_rad = 0.0f;
float32_t testDelta_rad = 0.005f;
uint32_t timer2Count = 0;
uint32_t timer2CountLast = 0;
uint32_t delataISR = 0;
float32_t fDelataISR = 0.0f;

__interrupt CODE_SECTION( "ramfuncs") void motor1CtrlISR(void)
{
    MOTOR_Vars_t *obj = (MOTOR_Vars_t *)motorHandle_M1;
    USER_Params *objUser = (USER_Params *)(obj->userParamsHandle);
    MATH_Vec2 phasor;

    obj->ISRCount++;

    // trigger XINT for absolute encoder
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP12);

	timer2Count = HAL_readTimerCnt(halHandle,HAL_CPU_TIMER2);;
	delataISR = timer2CountLast - timer2Count;
	fDelataISR = (float32_t)delataISR/160000000.0f;
	timer2CountLast = timer2Count;

    // Gets the current mechanical angular position of the motor
    obj->posRef_deg = obj->posRef_rad*180.0f/MATH_PI;
	obj->pos_deg = (float32_t)((((receivedChar[2]<<16)&0xFF0000)|((receivedChar[3]<<8)&0xFF00)|(receivedChar[4]&0xFF))/36000.0f);
//	obj->pos_deg = (float32_t)((((receivedChar[2]<<16)&0xFF0000)|((receivedChar[3]<<8)&0xFF00))/36000.0f);
	obj->posRef_min = obj->posRef_deg*60;
	obj->pos_min = obj->pos_deg*60;
	obj->posRef_sec = obj->posRef_min*60;
	obj->pos_sec = obj->pos_min*60;
	obj->pos_rad = obj->pos_deg*MATH_PI/180.0f;

	if((obj->pos_deg>obj->posLast_deg)&&(obj->direction == 1))
	{
		obj->posFilter_rad = obj->pos_rad;
	}
	else
	{
		obj->posFilter_rad = obj->posFilter_rad *0.995f + obj->pos_rad * 0.005f;
	}
	obj->posDelta_deg = obj->pos_deg - obj->posLast_deg;
	obj->posDelta_rad = obj->posDelta_deg*MATH_PI/180.0f;;
	obj->posLast_deg = obj->pos_deg;

    // Read the ADC data with offsets
    HAL_readMtr1ADCData(&obj->adcData);

    // remove offsets
    obj->adcData.I_A.value[0] -= obj->adcData.offset_I_ad.value[0] * obj->adcData.current_sf;
    obj->adcData.I_A.value[1] -= obj->adcData.offset_I_ad.value[1] * obj->adcData.current_sf;
    obj->adcData.I_A.value[2] -= obj->adcData.offset_I_ad.value[2] * obj->adcData.current_sf;

    obj->adcData.V_V.value[0] -= obj->adcData.offset_V_sf.value[0] * obj->adcData.VdcBus_V;
    obj->adcData.V_V.value[1] -= obj->adcData.offset_V_sf.value[1] * obj->adcData.VdcBus_V;
    obj->adcData.V_V.value[2] -= obj->adcData.offset_V_sf.value[2] * obj->adcData.VdcBus_V;

    // run Clarke transform on voltage
    CLARKE_run(obj->clarkeHandle_V,&obj->adcData.V_V, &obj->estInputData.Vab_V);

    // run Clarke transform on current
    CLARKE_run(obj->clarkeHandle_I, &obj->adcData.I_A, &obj->estInputData.Iab_A);

	if(obj->flagRunIdentAndOnLine == true)
	{
		obj->counterTrajSpeed++;

		if(obj->counterTrajSpeed >= objUser->numIsrTicksPerTrajTick)
		{
			// clear counter
			obj->counterTrajSpeed = 0;

			// run a trajectory for speed reference,
			// so the reference changes with a ramp instead of a step
			TRAJ_run(obj->trajHandle_spd);
		}

		obj->enableCurrentCtrl = obj->flagEnableCurrentCtrl;
		obj->enableSpeedCtrl = obj->flagEnableSpeedCtrl;

		// get Id reference for Rs OnLine
		obj->IdRated_A = EST_getIdRated_A(obj->estHandle);
	}
	else
	{
		obj->enableSpeedCtrl = false;
		obj->enableCurrentCtrl = false;
	}

	// store the input data into a buffer
	obj->estInputData.speed_ref_Hz = TRAJ_getIntValue(obj->trajHandle_spd);
	obj->speed_int_Hz = obj->estInputData.speed_ref_Hz;
	obj->estInputData.dcBus_V = obj->adcData.VdcBus_V;

    EST_run(obj->estHandle, &obj->estInputData, &obj->estOutputData);

    // compute angle with delay compensation
//    obj->angleDelta_rad = objUser->angleDelayed_sf_sec * obj->estOutputData.fm_lp_rps;
//    obj->angleEST_rad =MATH_incrAngle(obj->estOutputData.angle_rad, obj->angleDelta_rad);

    obj->angleEST_rad = MATH_incrAngle(obj->estOutputData.angle_rad, 0);
    obj->speedEST_Hz = EST_getFm_lp_Hz(obj->estHandle);
//    obj->speed_Hz = obj->speedEST_Hz;
    obj->speed_Hz = obj->speedENC_Hz;
    obj->speedFilter_Hz = obj->speedFilter_Hz *0.8f + obj->speed_Hz * 0.2f;
    obj->speedAbs_Hz = fabsf(obj->speedFilter_Hz);

    obj->oneOverDcBus_invV = obj->estOutputData.oneOverDcBus_invV;

    // Running state
    if(obj->motorState >= MOTOR_CL_RUNNING)
    {
    	obj->angleFOC_rad = -(obj->pos_rad*USER_MOTOR1_NUM_POLE_PAIRS - obj->angleENCOffset_rad);
    	obj->angleFOC_rad = MATH_incrAngle1(obj->angleFOC_rad,0);

    	obj->angleDelta_rad = obj->angleFOC_rad - obj->angleFOCLast_rad;

    	if(obj->direction == 1.0)
    	{
        	if(((obj->angleDelta_rad >= 0) && (obj->angleDelta_rad <= MATH_PI))||
        			((obj->angleDelta_rad < 0)&& (obj->angleDelta_rad <= -MATH_PI)))
        	{
        		obj->angleDelta_rad = MATH_incrAngle1(obj->angleDelta_rad,0);
        	}
    	}
    	else if(obj->direction == -1.0)
    	{
        	if(((obj->angleDelta_rad <= 0) && (obj->angleDelta_rad >= -MATH_PI))
        			||((obj->angleDelta_rad > 0)&& (obj->angleDelta_rad >= MATH_PI)))
        	{
        		obj->angleDelta_rad = MATH_incrAngle2(obj->angleDelta_rad,0);
        	}
    	}

    	obj->angleFOCLast_rad = obj->angleFOC_rad;
    	obj->angleDeltaSum_rad += obj->angleDelta_rad;
    }
    else if(obj->motorState == MOTOR_OL_START)
    {
        obj->angleFOC_rad = obj->angleEST_rad;
        obj->motorState = MOTOR_CL_RUNNING;
    }
    else if(obj->motorState == MOTOR_ALIGNMENT)
    {
        obj->angleFOC_rad = 0.0f;
        obj->enableSpeedCtrl = false;

        obj->stateRunTimeCnt++;

        obj->IsRef_A = 0.0f;
        obj->Idq_out_A.value[0] = obj->alignCurrent_A;
        obj->Idq_out_A.value[1] = 0.0f;

        TRAJ_setIntValue(obj->trajHandle_spd, 0.0f);

        if((obj->flagEnableAlignment == true)&&(obj->stateRunTimeCnt > obj->alignTimeDelay))
        {
            obj->angleENCOffset_rad = MATH_incrAngle1(obj->pos_deg*USER_MOTOR1_NUM_POLE_PAIRS*MATH_PI/180.0f, 0);
            obj->stateRunTimeCnt = 0;
//            obj->Idq_out_A.value[0] = 0.0f;
            PI_setUi(obj->piHandle_spd, 0.0);
        }
        else if(obj->flagEnableAlignment == false)
        {
        	obj->angleENCOffset_rad = 6.1354795;//0.28429508 // 2.0108056 // 1.9874787 // 4.566216
        	obj->stateRunTimeCnt = 0;
        	obj->motorState = MOTOR_CL_RUNNING;
            obj->Idq_out_A.value[0] = 0.0f;
            PI_setUi(obj->piHandle_spd, 0.0);
        }

//        if((obj->stateRunTimeCnt > obj->alignTimeDelay) ||
//                 (obj->flagEnableAlignment == false))
//        {
//            obj->stateRunTimeCnt = 0;
//            obj->angleENCOffset_rad = MATH_incrAngle1(obj->pos_rad*USER_MOTOR1_NUM_POLE_PAIRS*MATH_PI/180.0f, 0);
//            obj->motorState = MOTOR_CL_RUNNING;
//            obj->Idq_out_A.value[0] = 0.0f;
//            PI_setUi(obj->piHandle_spd, 0.0);
////            EST_setAngle_rad(obj->estHandle, obj->angleFOC_rad);
//        }
    }

    //************** Speed calculation **************//
    // encoder counter
    obj->ENCCount++;
	if(obj->ENCCount >= speedSampleCount)
	{
        obj->speedENC_Hz = (obj->angleDeltaSum_rad/ speedSampleTime)/MATH_TWO_PI;
//        obj->speedFilter_Hz = obj->speedFilter_Hz *0.9995f + obj->speedENC_Hz * 0.0005f;

        obj->ENCCount = 0;
        obj->angleDeltaSum_rad = 0;
	}

    if(obj->flagEnableOLPosCtrl == true)
    {
    	obj->flagEnableOLSpeedCtrl = false;
    	obj->flagEnableSpeedCtrl = false;
    	obj->flagEnableCurrentCtrl = false;

    	obj->Idq_out_A.value[0] = obj->IsRef_A;
    	obj->Idq_out_A.value[1] = 0.0f;

    	obj->IdqRef_A.value[0] = obj->Idq_out_A.value[0];
    	obj->IdqRef_A.value[1] = obj->Idq_out_A.value[1];

    	obj->Vdq_out_V.value[0] = obj->IdqRef_A.value[0];
    	obj->Vdq_out_V.value[1] = obj->IdqRef_A.value[1];

    	obj->angleFOC_rad = -MATH_incrAngle1(obj->posRef_rad*USER_MOTOR1_NUM_POLE_PAIRS, -obj->angleENCOffset_rad);
    }

    // run the position controller
    if(obj->flagEnablePosCtrl == true)
    {
    	PI_run(obj->piHandle_pos,obj->posRef_rad,obj->pos_rad,(float32_t *)&obj->speedRef_Hz);
    	obj->speedRef_Hz = -obj->speedRef_Hz;
    }

    // compute the sin/cos phasor
    if(obj->flagEnableOLCurrentCtrl == true)
    {
        testFoc_rad+=testDelta_rad;
        MATH_incrAngle1(testFoc_rad,0);
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
    PARK_run(obj->parkHandle_I, &(obj->estInputData.Iab_A),(MATH_vec2 *)&(obj->Idq_in_A));

    // run the speed controller
    obj->counterSpeed++;

    if(obj->counterSpeed >= objUser->numCtrlTicksPerSpeedTick)
    {
        obj->counterSpeed = 0;

        if(obj->enableSpeedCtrl == true)
        {
            PI_run(obj->piHandle_spd,
                   obj->speed_int_Hz,
                   obj->speedFilter_Hz,
                   (float32_t *)&obj->IsRef_A);
        }
        else if(((obj->motorState >= MOTOR_CL_RUNNING) &&
                (obj->flagMotorIdentified == true))||
        		(obj->flagEnableOLPosCtrl == true)||
				(obj->flagEnableOLSpeedCtrl == true))
        {
            if(obj->speed_int_Hz > 0.0f)
            {
                obj->IsRef_A = obj->IsSet_A;
            }
            else
            {
                obj->IsRef_A = -obj->IsSet_A;
            }

            // for switching back speed closed-loop control
            PI_setUi(obj->piHandle_spd, obj->IsRef_A);
        }
    }

    if(obj->enableCurrentCtrl == true)
    {
    	obj->flagEnableOLSpeedCtrl = false;
    	obj->flagEnableOLCurrentCtrl = false;

        obj->Idq_out_A.value[1] = obj->IsRef_A;

        obj->IdqRef_A.value[0] = obj->Idq_out_A.value[0] + obj->IdRated_A;
        obj->IdqRef_A.value[1] = obj->Idq_out_A.value[1];

        EST_updateId_ref_A(obj->estHandle, &obj->IdqRef_A.value[0]);

        // Maximum voltage output
        objUser->maxVsMag_V = objUser->maxVsMag_pu * obj->adcData.VdcBus_V;

        PI_setMinMax(obj->piHandle_Id,-objUser->maxVsMag_V, objUser->maxVsMag_V);
        PI_run(obj->piHandle_Id, obj->IdqRef_A.value[0],
               obj->Idq_in_A.value[0], (float32_t*)&obj->Vdq_out_V.value[0]);

        // calculate Iq controller limits
        float32_t outMax_V = __sqrt((objUser->maxVsMag_V * objUser->maxVsMag_V) -
                          (obj->Vdq_out_V.value[0] * obj->Vdq_out_V.value[0]));

        PI_setMinMax(obj->piHandle_Iq, -outMax_V, outMax_V);
        PI_run(obj->piHandle_Iq, obj->IdqRef_A.value[1],
               obj->Idq_in_A.value[1], (float32_t*)&obj->Vdq_out_V.value[1]);
    }
    else if(obj->flagEnableOLCurrentCtrl == true)
    {
    	obj->flagEnableSpeedCtrl = false;
    	obj->flagEnableCurrentCtrl = false;

    	obj->Idq_out_A.value[0] = 0.0f;
    	obj->Idq_out_A.value[1] = obj->IsRef_A;

    	obj->IdqRef_A.value[0] = obj->Idq_out_A.value[0];
    	obj->IdqRef_A.value[1] = obj->Idq_out_A.value[1];

    	obj->Vdq_out_V.value[0] = obj->IdqRef_A.value[0];
    	obj->Vdq_out_V.value[1] = obj->IdqRef_A.value[1];
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
    SVGEN_setup(obj->svgenHandle, obj->oneOverDcBus_invV);

    // run the space vector generator (SVGEN) module
    SVGEN_run(obj->svgenHandle, &obj->Vab_out_V, &(obj->pwmData.Vabc_pu));
#endif
//    if(HAL_getPwmEnableStatus(obj->halMtrHandle) == false)
//    {
//        // clear PWM data
//        obj->pwmData.Vabc_pu.value[0] = 0.0f;
//        obj->pwmData.Vabc_pu.value[1] = 0.0f;
//        obj->pwmData.Vabc_pu.value[2] = 0.0f;
//    }

    // write the PWM compare values
    HAL_writePWMData(obj->halMtrHandle, &obj->pwmData);

    // Collect current and voltage data to calculate the RMS value
    collectRMSData(motorHandle_M1);

    // acknowledge the ADC interrupt
    HAL_ackMtr1ADCInt();

    // DEBUG
    dbLoop++;
    if(dbLoop >= 10)
    {
        if(dbCount>=200)
        {
        	dbCount = 0;
        }

//        debug1[dbCount] = obj->adcData.I_A.value[0];
//        debug1[dbCount] = obj->angleDelta_rad;
        debug1[dbCount] = obj->angleFOC_rad;
        debug2[dbCount] = obj->pos_rad;
//        debug3[dbCount] = obj->pos_rad;
        dbCount++;
        dbLoop = 0;
    }

    return;
} // end of motor1CtrlISR() function

