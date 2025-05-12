/*
 * motor1_drive.h
 *
 *  Created on: 2023Äê1ÔÂ31ÈÕ
 *      Author: Kangaihong
 */

#ifndef SRC_CONTROL_MOTOR1_DRIVE_H_
#define SRC_CONTROL_MOTOR1_DRIVE_H_

#include "userParams.h"
#include "motor_common.h"

// the globals
extern volatile MOTOR_Handle motorHandle_M1;
extern volatile MOTOR_Vars_t motorVars_M1;
extern volatile MOTOR_SetVars_t motorSetVars_M1;
extern volatile USER_Params userParams_M1;

extern void initMotor1Handles(MOTOR_Handle smoHandle);
extern void initMotor1CtrlParameters(MOTOR_Handle smoHandle);
extern void runMotor1OffsetsCalculation(MOTOR_Handle smoHandle);
extern void runMotor1Control(MOTOR_Handle smoHandle);

#endif /* SRC_CONTROL_MOTOR1_DRIVE_H_ */
