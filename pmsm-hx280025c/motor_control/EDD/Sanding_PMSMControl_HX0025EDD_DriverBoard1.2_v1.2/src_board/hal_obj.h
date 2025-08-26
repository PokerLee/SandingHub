//*****************************************************************************
//
//! \file hal_obj.h
//!
//! \brief 外设结构体相关
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

#ifndef HAL_OBJ_H
#define HAL_OBJ_H

#include "device.h"
#include "hal_data.h"
#include "drv8323s.h"

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
//
//! \addtogroup HALL_OBJ
//! @{
//
//*****************************************************************************

// Defines for DRV ICs
#define DRVIC_Obj               DRV8323_Obj
#define DRVIC_VARS_t            DRV8323_VARS_t
#define DRVIC_Handle            DRV8323_Handle
#define DRVICVARS_Handle        DRV8323VARS_Handle

#define DRVIC_init              DRV8323_init
#define DRVIC_enable            DRV8323_enable
#define DRVIC_writeData         DRV8323_writeData
#define DRVIC_readData          DRV8323_readData

#define DRVIC_setupSPI          DRV8323_setupSPI
#define DRVIC_setSPIHandle      DRV8323_setSPIHandle
#define DRVIC_setGPIOCSNumber   DRV8323_setGPIOCSNumber
#define DRVIC_setGPIOENNumber   DRV8323_setGPIOENNumber


//! \brief      Defines the HAL object
typedef struct _HAL_MTR_Obj_
{
  uint32_t       pwmHandle[3];       //<! the PWM handles
  uint32_t       cmpssHandle[3];     //!< the CMPSS handle
  uint32_t		 eqepHandle;		 //!< the EQEP handle
  uint32_t       spiHandle;          //!< the SPI handle

  DRVIC_Handle   drvicHandle;        //!< the drvic interface handle
  DRVIC_Obj      drvic;              //!< the drvic interface object

  bool           flagEnablePWM;      //<! the pwm enable flag
  uint32_t       gateEnableGPIO;
  uint32_t       gateCalGPIO;
  uint16_t       numCurrentSensors;  //!< the number of current sensors
  uint16_t       numVoltageSensors;  //!< the number of voltage sensors

} HAL_MTR_Obj;

//! \brief      Defines the HAL_MTR handle
//! \details    The HAL_MTR handle is a pointer to a HAL_MTR object.  In all
//!             HAL_MTR functions, the HAL_MTR handle is passed so that the
//!             function knows what peripherals are to be accessed.
//!
typedef struct _HAL_MTR_Obj_ *HAL_MTR_Handle;


//! \brief      Defines the hardware abstraction layer (HAL) data
//! \details    The HAL object contains all handles to peripherals.  When accessing a
//!             peripheral on a processor, use a HAL function along with the HAL handle
//!             for that processor to access its peripherals.
//!
typedef struct _HAL_Obj_
{
  uint32_t       adcHandle[2];      //!< the ADC handles
  uint32_t       adcResult[2];      //!< the ADC results

  uint32_t       timerHandle[3];    //<! the timer handles
  uint32_t 		 canHandle;			//!< the CAN handle
  uint32_t       linHandle;         //!< the LIN handle

  uint32_t       dmaHandle;         //!< the DMA handle
  uint32_t       dmaChHandle[4];    //!< the DMA Channel handle

  HAL_MTR_Handle mtrHandle[2];      //!< the motor control interface handle
} HAL_Obj;

//! \brief      Defines the HAL handle
//! \details    The HAL handle is a pointer to a HAL object.  In all HAL functions
//!             the HAL handle is passed so that the function knows what peripherals
//!             are to be accessed.
//!
typedef struct _HAL_Obj_ *HAL_Handle;


//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // end of HAL_OBJ_H definition

