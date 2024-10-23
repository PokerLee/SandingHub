//*****************************************************************************
//
//! \file motor_common.h
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

#ifndef MOTOR_COMMON_H
#define MOTOR_COMMON_H

#include "hal.h"
#include "userParams.h"
#include "est.h"
#include "user.h"
#include "traj.h"
#if MTR1_SV_MODE == 5
#include "svgen.h"
#endif
#if MTR1_SV_MODE == 7
#include "svpwm.h"
#endif
#include "transform.h"

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

extern HAL_Handle    halHandle;
extern HAL_Obj       hal;

//*****************************************************************************
// defines
//*****************************************************************************

#define M_OVER_VOLTAGE_BIT          0x0001    // DC Bus Over Voltage Fault
#define M_UNDER_VOLTAGE_BIT         0x0002    // DC Bus Under Voltage Fault
#define M_MOTOR_OVER_TEMPER_BIT     0x0004    // Motor over temperature Fault
#define M_MODULE_OVER_TEMPER_BIT    0x0008    // Module over temperature Fault

#define M_MODULE_OVER_CURRENT_BIT   0x0010    // Hardware Over Current Fault
#define M_OVER_PEAK_CURRENT_BIT     0x0020    // internal CMPSS Over Current Fault
#define M_MOTOR_OVER_LOAD_BIT       0x0040    // Over Load Error
#define M_MOTOR_LOST_PHASE_BIT      0x0080    // Motor Lost Phase

#define M_CURRENT_UNBALANCE_BIT     0x0100    // Motor Phase Current Unbalance
#define M_MOTOR_STALL_BIT           0x0200    // Motor Stall
#define M_STARTUP_FAILE_BIT         0x0400    // Startup failed
#define M_MOTOR_OVER_SPEED_BIT      0x0800    // Motor Over Speed

#define M_RESERVE_12_BIT            0x1000    // Reserved for further use
#define M_RESERVE_13_BIT            0x2000    // Reserved for further use
#define M_CURRENT_OFFSET_BIT        0x4000    // Current offsets
#define M_VOLTAGE_OFFSET_BIT        0x8000    // voltage offsets

#define M_MASK_ALL_FAULT_BITS       0x0000
#define M_ENABLE_ALL_FAULT_BITS     0xFFFF

//! \brief Enable all over current protection
#define MTR_FAULT_ENABLE_OC_OUV         M_OVER_VOLTAGE_BIT                     \
                                      + M_UNDER_VOLTAGE_BIT                    \
                                      + M_MODULE_OVER_CURRENT_BIT              \
                                      + M_OVER_PEAK_CURRENT_BIT                \
                                      + M_CURRENT_OFFSET_BIT                   \
                                      + M_VOLTAGE_OFFSET_BIT

//! \brief Enable all fault protection
#define MTR_FAULT_ENABLE_ALL            M_OVER_VOLTAGE_BIT                     \
                                      + M_UNDER_VOLTAGE_BIT                    \
                                      + M_MOTOR_OVER_TEMPER_BIT                \
                                      + M_MODULE_OVER_TEMPER_BIT               \
                                      + M_MODULE_OVER_CURRENT_BIT              \
                                      + M_OVER_PEAK_CURRENT_BIT                \
                                      + M_MOTOR_OVER_LOAD_BIT                  \
                                      + M_MOTOR_LOST_PHASE_BIT                 \
                                      + M_CURRENT_UNBALANCE_BIT                \
                                      + M_MOTOR_STALL_BIT                      \
                                      + M_STARTUP_FAILE_BIT                    \
                                      + M_MOTOR_OVER_SPEED_BIT                 \
                                      + M_CURRENT_OFFSET_BIT                   \
                                      + M_VOLTAGE_OFFSET_BIT

//! \brief Clear all fault protection except over/under voltage and offset error
#define MTR_FAULT_CLEAR                 M_OVER_VOLTAGE_BIT                     \
                                      + M_UNDER_VOLTAGE_BIT                    \
                                      + M_MOTOR_OVER_TEMPER_BIT                \
                                      + M_MODULE_OVER_TEMPER_BIT

//! \brief Clear all fault protection
#define MTR_FAULT_DISABLE_ALL           0x0000

//! \brief User motor1 fault configuration
#define MTR1_FAULT_MASK_SET             MTR_FAULT_ENABLE_OC_OUV

//! \brief Set motor1 structure
typedef struct _MOTOR_SetVars_t_
{
    float32_t Rr_Ohm;
    float32_t Rs_Ohm;
    float32_t Ls_d_H;
    float32_t Ls_q_H;
    float32_t flux_VpHz;
    float32_t flux_Wb;
    float32_t RoverL_rps;

    float32_t RsOnLine_Ohm;

    float32_t Kp_pos;
    float32_t Ki_pos;

    float32_t Kp_spd;
    float32_t Ki_spd;

    float32_t Kp_Id;
    float32_t Ki_Id;

    float32_t Kp_Iq;
    float32_t Ki_Iq;

//    float32_t Kp_fwc;
//    float32_t Ki_fwc;
//    float32_t angleFWCMax_rad;

//    float32_t overModulation;
    float32_t RsOnLineCurrent_A;
    float32_t magneticCurrent_A;

    float32_t lostPhaseSet_A;
    float32_t unbalanceRatioSet;
    float32_t overLoadSet_W;
    float32_t stallCurrentSet_A;

    float32_t IsFailedChekSet_A;

    float32_t speedFailMaxSet_Hz;
    float32_t speedFailMinSet_Hz;

    float32_t toqueFailMinSet_Nm;

    float32_t maxPeakCurrent_A;
    float32_t overCurrent_A;
    float32_t currentInv_sf;

    float32_t overVoltageFault_V;
    float32_t overVoltageNorm_V;
    float32_t underVoltageFault_V;
    float32_t underVoltageNorm_V;

    uint16_t voltageFaultTimeSet;
    uint16_t overLoadTimeSet;
    uint16_t motorStallTimeSet;
    uint16_t unbalanceTimeSet;
    uint16_t lostPhaseTimeSet;
    uint16_t overSpeedTimeSet;
    uint16_t startupFailTimeSet;

    uint16_t overCurrentTimesSet;

    uint16_t RsOnlineWaitTimeSet;
    uint16_t RsOnlineWorkTimeSet;

    uint16_t stopWaitTimeSet;
    uint16_t restartWaitTimeSet;
    uint16_t restartTimesSet;

    uint16_t  dacCMPValH;
    uint16_t  dacCMPValL;

} MOTOR_SetVars_t;

//! \brief Defines the MOTOR_SetVars_t handle
typedef struct _MOTOR_SetVars_t_ *MOTORSETS_Handle;

typedef struct _FAULT_MTR_BITS_
{             // bits  description
    uint16_t overVoltage:1;         // 0  DC Bus Over Voltage Fault
    uint16_t underVoltage:1;        // 1  DC Bus Under Voltage Fault
    uint16_t motorOverTemp:1;       // 2  Motor over temperature Fault
    uint16_t moduleOverTemp:1;      // 3  Power module over temperature Fault

    uint16_t moduleOverCurrent:1;   // 4  Hardware Over Current Fault Flag
    uint16_t overPeakCurrent:1;     // 5  internal CMPSS Over Current Fault Flag
    uint16_t overLoad:1;            // 6  Over Load Error
    uint16_t motorLostPhase:1;      // 7  Motor Lost Phase

    uint16_t currentUnbalance:1;    // 8  Motor Phase Current imbalance
    uint16_t motorStall:1;          // 9  Motor Stall
    uint16_t startupFailed:1;       // 10 Startup failed
    uint16_t overSpeed:1;           // 11 Motor Over Speed

    uint16_t reserve12:1;           // 12 Reserved
    uint16_t reserve13:1;           // 13 Reserved
    uint16_t currentOffset:1;       // 14 Current offset check
    uint16_t voltageOffset:1;       // 15 voltage offset check
} FAULT_MTR_BITS;

typedef union _FAULT_MTR_REG_t
{
    uint16_t        all;
    FAULT_MTR_BITS  bit;
}FAULT_MTR_REG_t;

typedef struct _MOTOR_Vars_t_
{
	// 中断计数器
	uint32_t ISRCount;						//!< 中断计数器
	uint32_t ISRPeriodCount;
	uint32_t ISRPeriodCountPrev;
	uint32_t ISRPeriodCountDelta;
	float32_t ISRPeriodCountDelta_f;

	uint32_t levelCount_180;				//!< 全站仪180度水平计数器
	uint16_t counterPos;
    uint16_t counterSpeed;
    uint16_t counterTrajSpeed;
    uint16_t stateRunTimeCnt;
//    uint16_t forceRunTimeDelay;
    uint16_t alignTimeDelay;
//    uint16_t fwcTimeDelay;
    uint16_t flyingStartTimeDelay;

    uint16_t overVoltageTimeCnt;
    uint16_t underVoltageTimeCnt;
    uint16_t overCurrentTimesCnt;
    uint16_t overLoadTimeCnt;
    uint16_t motorStallTimeCnt;
    uint16_t unbalanceTimeCnt;
    uint16_t lostPhaseTimeCnt;
    uint16_t overSpeedTimeCnt;
    uint16_t startupFailTimeCnt;
    uint16_t RsOnlineTimeCnt;

//    uint16_t powerOnTimeCnt;
    uint16_t stopWaitTimeCnt;
    uint16_t restartTimesCnt;
    uint16_t startSumTimesCnt;

	// 用户控制
	bool flagEnableShowMode;				//!< 启动演示模式

	// 电机控制
    bool flagEnableRunAndIdentify;
    bool flagRunIdentAndOnLine;
    bool flagEnableMotorIdentify;
    bool flagMotorIdentified;
    bool flagEnableRestart;

    bool flagEnableForceAngle;
    bool flagEnableOffsetCalc;
    bool flagEnableAlignment;

//    bool flagEnablePosCtrl;
//    bool flagEnableSpeedCtrl;
//    bool flagEnableOpenLoopCtrl;

    bool enableOpenLoopCtrl;
    bool enablePosCtrl;
    bool enableSpeedCtrl;
    bool enableCurrentCtrl;
    bool flagEnableCurrentCtrl;

//    bool flagEnableSpeedCtrl;
//    bool flagEnableCurrentCtrl;
//    bool enableOLSpeedCtrl;
//    bool enableOLCurrentCtrl;

    bool flagSetupController;

    bool flagEnableRsRecalc;
    bool flagEnablePowerWarp;
    bool flagBypassLockRotor;
    bool flagEnableRsOnLine;
    bool flagStartRsOnLine;
    bool flagRsOnLineContinue;

//    bool flagEnableFlyingStart;
//    bool flagStateFlyingStart;
    bool flagEnableBraking;

//    bool flagEnableIPD;
//    bool flagEnableFWC;
//    bool flagEnableMTPA;
//    bool flagUpdateMTPAParams;

    bool flagClearFaults;
    bool flagVIrmsCal;

    MOTOR_Status_e motorState;
    MCTRL_State_e  mctrlState;

	// 正倒镜180度控制
	uint8_t   fowardLevelCtrl;
	uint8_t   backLevelCtrl;
	uint8_t	  flagLevelPosSet;

	float32_t directionRef;                    //!< 1.0f->forward, -1.0f->reserve
	float32_t direction;                    //!< 1.0f->forward, -1.0f->reserve

    // Position loop variables
    uint32_t  posCount;
    float32_t pos_machine;
    float32_t posRef_rad;					//!< Refer to Mechanical angular position information, rad
    float32_t posRefPrev_rad;				//!< Refer to Mechanical angular position information, rad
    float32_t pos_rad;						//!< Current mechanical Angle position information, rad
    float32_t posPrev_rad;					//!< Current mechanical Angle position information, rad
    float32_t posDelta_rad;
    uint32_t  posRef_deg;					//!< Refer to Mechanical angular position information, degree
    uint32_t  posRef_min;					//!< Refer to Mechanical angular position information, minute
    uint32_t  posRef_sec;					//!< Refer to Mechanical angular position information, second
    uint32_t  pos_deg;						//!< Current mechanical Angle position information, degree
    uint32_t  pos_min;						//!< Current mechanical Angle position information, minute
    uint32_t  pos_sec;						//!< Current mechanical Angle position information, second

    bool flagPosSwitch;						//!<
    bool flagPosStable;						//!<

    // Speed loop variables
    float32_t speedRef_Hz;					//!< Speed reference value, Hz
    float32_t speed_int_Hz;                 //!< Set speed after ramp, Hz
    float32_t speed_Hz;						//!< Current speed, Hz
    float32_t speed_rps;					//!< Current speed, rpm
    float32_t speedENC_Hz;					//!< the speed from Encoder module
    float32_t speedFilter_Hz;				//!<
    float32_t speedAbs_Hz;					//!<
    float32_t speedStart_Hz;				//!<
    float32_t speedForce_Hz;				//!<
    float32_t speedFlyingStart_Hz;			//!<
    float32_t speedEST_Hz;					//!< the speed from EST module
//    float32_t speedPLL_Hz;					//!< the speed from PLL module
//    float32_t speedHall_Hz;					//!< the speed from Hall Sensor
//    float32_t speedINT_Hz;					//!< the speed from ISBLDC module

    float32_t accelerationMax_Hzps;			//!<
    float32_t accelerationStart_Hzps;		//!<

    // 电角度相关变量
    uint32_t ENCCount;						//!<
    float32_t angleDelayed_sf;				//!<
    float32_t angleDelta_rad;				//!< 本次控制周期相较上次控制周期的电角度delta值
    float32_t angleDeltaFilter_rad; 		//!< angleDelta_rad经一阶滤波后得到的值
    float32_t angleDeltaSum_rad;			//!< angleDelta_rad多次控制周期下的累加和
    float32_t angleENC_rad;					//!< 旋转电机编码器电角度值(rad)
    float32_t angleENCOffset_rad;			//!< 电机Id轴对齐后，电角度的偏移值
	float32_t angleEST_rad;					//!< 旋转电机估算电角度值(rad)
	float32_t angleESTDelta_rad;			//!< 本次控制周期相较上次控制周期的电角度delta值
//	float32_t anglePLL_rad;					//!< the rotor angle from PLL modules
//	float32_t angleENC_rad;					//!< the rotor angle from Encoder modules
//	float32_t angleComp_rad;				//!< the rotor angle delay compensation value
//	float32_t angleGen_rad;					//!< the rotor angle from Generator modules
//	float32_t angleHall_rad;				//!< the rotor angle from Hall Sensor
//	float32_t angleBrake_rad;				//!< the rotor angle for braking
//	float32_t angleFWC_rad;					//!<
	float32_t angleCurrent_rad;				//!<
//	float32_t angleOffsetIPD_rad;			//!<
//	float32_t angleDetectIPD_rad;			//!<
    float32_t angleFOC_rad;					//!< 旋转电机电角度值(rad)
    float32_t angleFOCPrev_rad;				//!< 上次控制周期的旋转电机电角度值(rad)

    FAULT_MTR_REG_t faultMtrNow;
    FAULT_MTR_REG_t faultMtrUse;
    FAULT_MTR_REG_t faultMtrMask;
    FAULT_MTR_REG_t faultMtrPrev;

    float32_t Is_A;							//!<
    float32_t Vs_V;							//!<
    float32_t VsRef_pu;						//!<
    float32_t VsRef_V;						//!<
    float32_t oneOverDcBus_invV;    		//!< the DC Bus inverse, 1/V

    float32_t IdRated_A;					//!<
    float32_t IsRef_A;						//!<
    float32_t IsSet_A;						//!<

    float32_t fluxCurrent_A;				//!<
    float32_t alignCurrent_A;				//!<
    float32_t startCurrent_A;				//!<
    float32_t maxCurrent_A;					//!<
    float32_t brakingCurrent_A;				//!<

    float32_t torque_Nm;					//!<

    MATH_Vec2 Vab_out_V;             		//!< the output control voltage on alpha&beta axis
    MATH_Vec2 Vdq_out_V;            		//!< the output control voltage on d&q axis
    MATH_Vec2 Vdq_offset_V;         		//!< the output offset voltage on d&q axis
    MATH_Vec2 Iab_A;                		//!< the alpha/beta current values, A
    MATH_Vec2 Vab_V;                		//!< the alpha/beta current values, V

    MATH_Vec2 Idq_in_A;						//!< the d&q axis current are converter from 3-phase sampling input current of motor
    MATH_Vec2 IdqRef_A;						//!< the reference current on d&q rotation axis
    MATH_Vec2 Idq_out_A;					//!< the reference output current on d&q rotation axis

    MotorNum_e motorNum;

    MOTORSETS_Handle motorSetsHandle;

    userParams_Handle   userParamsHandle;

    HAL_MTR_Handle halMtrHandle;			//!< the handle for the hardware abstraction layer to motor control

    DRVICVARS_Handle drvVarsHandle;

    SAMPLE_Mode_e sampleMode;

    HAL_ADCData_t adcData;
    HAL_PWMData_t pwmData;

    EST_InputData_t 	estInputData;
    EST_OutputData_t 	estOutputData;
    EST_State_e 		estState;
    EST_Traj_State_e 	trajState;
    EST_Handle  		estHandle;			//!< the handle for the estimator

    CLARKE_Handle clarkeHandle_V;			//!< the handle for the voltage Clarke transform
    CLARKE_Handle clarkeHandle_I;			//!< the handle for the current Clarke transform

    IPARK_Handle  iparkHandle_V;			//!< the handle for the inverse Park transform

    PARK_Handle   parkHandle_I;				//!< the handle for the Park object
    PARK_Handle   parkHandle_V;				//!< the handle for the Park object

//	SVM_Mode_e 	 svmMode;
	TRAJ_Handle  trajHandle_spd;			//!< the handle for the speed reference trajectory

    PI_Handle     piHandle_pos;				//!< the handle for the speed position controller
    PI_Handle     piHandle_spd;				//!< the handle for the speed PI controller
    PI_Handle     piHandle_Id;				//!< the handle for the Id PI controller
    PI_Handle     piHandle_Iq;				//!< the handle for the Iq PI controller

    SVGEN_Handle  svgenHandle;				//!< the handle for the space vector generator

    uint16_t VIrmsIsrSet;
    uint16_t VIrmsIsrCnt;

    FlyingStart_Mode_e flyingStartMode;
    BRAKE_Mode_e brakingMode;
    OPERATE_Mode_e operateMode;

    // 有效值计算
//    float32_t frswPos_sf;					//!<
    float32_t VIrmsIsrScale;
    float32_t IrmsCalSF;
    float32_t VrmsCalSF;
    float32_t power_sf;

    float32_t IrmsCalSum[3];
    float32_t IrmsPrdSum[3];
    float32_t Irms_A[3];

    float32_t VrmsCalSum[3];
    float32_t VrmsPrdSum[3];
    float32_t Vrms_V[3];

    float32_t unbalanceRatio;
    float32_t powerActive_W;
    float32_t powerReal_W;
}MOTOR_Vars_t;

//! \brief Defines the MOTOR_Vars_t handle
typedef struct _MOTOR_Vars_t_ *MOTOR_Handle;

//! \brief  Get the controllers Parameters
static inline void getControllers(MOTOR_Handle handle)
{
    MOTOR_Vars_t *obj = (MOTOR_Vars_t *)handle;
    MOTOR_SetVars_t *objSets = (MOTOR_SetVars_t *)(handle->motorSetsHandle);

    // Get the Id controller parameters
    objSets->Kp_Id = PI_getKp(obj->piHandle_Id);
    objSets->Ki_Id = PI_getKi(obj->piHandle_Id);

    // Get the Iq controller parameters
    objSets->Kp_Iq = PI_getKp(obj->piHandle_Iq);
    objSets->Ki_Iq = PI_getKi(obj->piHandle_Iq);

    // Get the speed controller parameters
    objSets->Kp_spd = PI_getKp(obj->piHandle_spd);
    objSets->Ki_spd = PI_getKi(obj->piHandle_spd);

    // Get the speed controller parameters
    objSets->Kp_pos = PI_getKp(obj->piHandle_pos);
    objSets->Ki_pos = PI_getKi(obj->piHandle_pos);

}

static inline void calcMotorOverCurrentThreshold(MOTOR_Handle handle)
{
    MOTOR_SetVars_t *obj = (MOTOR_SetVars_t *)(handle->motorSetsHandle);

    float32_t overCurrent_A;

    overCurrent_A = obj->overCurrent_A;

    if(overCurrent_A > obj->maxPeakCurrent_A)
    {
        overCurrent_A = obj->maxPeakCurrent_A;
    }

    int16_t cmpValue = (int16_t)(overCurrent_A * obj->currentInv_sf);

    obj->dacCMPValH = 2048 + cmpValue;
    obj->dacCMPValL = 2048 - cmpValue;

    return;
}

//! \brief set up pi controller
extern void setupControllers(MOTOR_Handle handle);

//! \brief run motor monitor in main loop timer
extern void runMotorMonitor(MOTOR_Handle handle);

//! \brief  Calculate the RMS data
extern void calculateRMSData(MOTOR_Handle handle);

//! \brief  Resets motor control parameters for restarting motor
extern void resetMotorControl(MOTOR_Handle handle);

//! \brief  Sets up control parameters for stopping motor
extern void stopMotorControl(MOTOR_Handle handle);

//! \brief  Position check monitor
extern void posCheckMoinitor(MOTOR_Handle handle);

//! \brief  Sets up control parameters for restarting motor
extern void restartMotorControl(MOTOR_Handle handle);

//! \brief  Update global variables
extern void updateGlobalVariables(MOTOR_Handle handle);

//! \brief  Collect the current and voltage data to calculate the RMS
extern void collectRMSData(MOTOR_Handle handle);

//! \brief Rs online calibration
extern void runRsOnLine(MOTOR_Handle handle);

extern void setupClarke_I(CLARKE_Handle handle, const uint16_t numCurrentSensors);
extern void setupClarke_V(CLARKE_Handle handle,const uint16_t numVoltageSensors);
extern void CPU_TIME_reset(CPU_TIME_Handle handle);

//! \brief  Update the controllers
static inline void updateControllers(MOTOR_Handle handle)
{
    MOTOR_Vars_t *obj = (MOTOR_Vars_t *)handle;
    MOTOR_SetVars_t *objSets = (MOTOR_SetVars_t *)(handle->motorSetsHandle);

    if(obj->mctrlState >= MCTRL_FIRST_RUN)
    {
	   if(handle->flagEnableShowMode)
	   {
	        // update the Id controller
	        PI_setGains(obj->piHandle_Id,objSets->Kp_Id , objSets->Ki_Id);//objSets->Kp_Id

	        // update the Iq controller
	        PI_setGains(obj->piHandle_Iq, objSets->Kp_Iq, objSets->Ki_Iq);

	        // update the speed controller
	        PI_setGains(obj->piHandle_spd, objSets->Kp_spd, objSets->Ki_spd);

	        // update the position controller
	        PI_setGains(obj->piHandle_pos, objSets->Kp_pos, objSets->Ki_pos);

	        PI_setMinMax(obj->piHandle_pos,-10,10);

//	        if(handle->fowardLevelCtrl == 1)
//			{
//	        	PI_setMinMax(obj->piHandle_spd,0,13);
//			}
//	        if(handle->backLevelCtrl == 1)
//			{
//	        	PI_setMinMax(obj->piHandle_spd,-13,0);
//			}

	   }
	   else
	   {
	        // update the Id controller
	        PI_setGains(obj->piHandle_Id,objSets->Kp_Id , objSets->Ki_Id);//objSets->Kp_Id

	        // update the Iq controller
	        PI_setGains(obj->piHandle_Iq, objSets->Kp_Iq, objSets->Ki_Iq);

	        // update the speed controller
	        PI_setGains(obj->piHandle_spd, objSets->Kp_spd, objSets->Ki_spd);

	        // update the position controller
	        PI_setGains(obj->piHandle_pos, objSets->Kp_pos, objSets->Ki_pos);
	   }
    }
}

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // end of MOTOR_COMMON_H definition
