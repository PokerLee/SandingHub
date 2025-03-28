//*****************************************************************************
//
//! \file pi.h
//!
//! \brief PI控制器相关
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

#include "user.h"

// **************************************************************************
// the defines


// **************************************************************************
// the globals


// **************************************************************************
// the functions

void  HAL_disableGlobalInts()
{
    // disable global interrupts
    Interrupt_disableMaster();

    return;
} // end of HAL_disableGlobalInts() function

void HAL_disableWdog()
{
    // disable watchdog
    SysCtl_disableWatchdog();

    return;
} // end of HAL_disableWdog() function

void HAL_enableCtrlInts()
{
    // Acknowledge interrupt from PIE group
    Interrupt_clearACKGroup(MTR1_INT_ACK_GROUP);
//    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP12);

    // Enable external interrupt
    Interrupt_enable(INT_XINT3);

    // enable the PIE interrupts associated with the ADC interrupts
    Interrupt_enable(MTR1_PIE_INT_NUM);    // motor_1

    // enable the ADC interrupts for motor_1
    ADC_enableInterrupt(MTR1_ADC_INT_BASE, MTR1_ADC_INT_NUM);

    // enable the cpu interrupt for PWM and interrupts
    Interrupt_enableInCPU(MTR1_CPU_INT_NUM);        // motor_1

    return;
} // end of HAL_enableCtrlInts() function


void HAL_enableDebugInt()
{

    // enable debug events
    ERTM;

    return;
} // end of HAL_enableDebugInt() function

void HAL_enableGlobalInts()
{

    // enable global interrupts
    Interrupt_enableMaster();

    return;
} // end of HAL_enableGlobalInts() function


void HAL_init(void *pMemory)
{
    HAL_Handle handle;
    HAL_Obj *obj;

    // assign the handle
    handle = (HAL_Handle)pMemory;

    // assign the object
    obj = (HAL_Obj *)handle;

    // disable watchdog
    SysCtl_disableWatchdog();

    // Two ADC modules in this device
    // initialize the ADC handles
    obj->adcHandle[0] = ADCA_BASE;
    obj->adcHandle[1] = ADCC_BASE;

    // initialize the ADC results
    obj->adcResult[0] = ADCARESULT_BASE;
    obj->adcResult[1] = ADCCRESULT_BASE;

    // No DAC modules in this device
    // No CLA module in this device

    // initialize SCI handle
    obj->sciHandle = SCIA_BASE;             //!< the SCIA handle

    // initialize DMA handle
    obj->dmaHandle = DMA_BASE;              //!< the DMA handle

    // initialize SPI handle
  //  obj->spiHandle[0] = SPIA_BASE; //kang add
  // obj->spiHandle[1] = SPIB_BASE; //kang add
    // initialize DMA channel handle
    obj->dmaChHandle[0] = DMA_CH3_BASE;     //!< the DMA Channel handle
    obj->dmaChHandle[1] = DMA_CH4_BASE;     //!< the DMA Channel handle
    obj->dmaChHandle[2] = DMA_CH5_BASE;     //!< the DMA Channel handle
    obj->dmaChHandle[3] = DMA_CH6_BASE;     //!< the DMA Channel handle

    // initialize timer handles
    obj->timerHandle[0] = CPUTIMER0_BASE;
    obj->timerHandle[1] = CPUTIMER1_BASE;
    obj->timerHandle[2] = CPUTIMER2_BASE;

} // end of HAL_init() function

HAL_MTR_Handle HAL_MTR1_init(void *pMemory, const size_t numBytes)
{
    HAL_MTR_Handle handle;
    HAL_MTR_Obj *obj;

    if(numBytes < sizeof(HAL_MTR_Obj))
    {
        return((HAL_MTR_Handle)NULL);
    }

    // assign the handle
    handle = (HAL_MTR_Handle)pMemory;

    // assign the object
    obj = (HAL_MTR_Obj *)handle;

    // initialize PWM handles for Motor 1
    obj->pwmHandle[0] = MTR1_PWM_U_BASE;        //!< the PWM handle
    obj->pwmHandle[1] = MTR1_PWM_V_BASE;        //!< the PWM handle
    obj->pwmHandle[2] = MTR1_PWM_W_BASE;        //!< the PWM handle

    obj->cmpssHandle[0] = MTR1_CMPSS_U_BASE;    //!< the CMPSS handle
    obj->cmpssHandle[1] = MTR1_CMPSS_V_BASE;    //!< the CMPSS handle
    obj->cmpssHandle[2] = MTR1_CMPSS_W_BASE;    //!< the CMPSS handle

    obj->eqepHandle = MTR1_EQEP_ENCODER_BASE;	//!< the EQEP handle

    // Assign gateEnableGPIO

    // initialize drv8323 interface
    obj->drvicHandle = DRVIC_init(&obj->drvic);

    // initialize SPI handle
    obj->spiHandle = MTR1_SPI_BASE;             //!< the SPI handle

    obj->gateEnableGPIO = MTR1_GATE_EN_GPIO;
    obj->gateCalGPIO = MTR1_GATE_CAL_GPIO;
    // BSXL8323RS_REVA
    return(handle);
} // end of HAL_MTR1_init() function

void HAL_setParams(HAL_Handle handle)
{
    // disable global interrupts
    Interrupt_disableMaster();

    // setup the GPIOs
    HAL_setupGPIOs(handle);

    // setup the ADCs
    HAL_setupADCs(handle);

    // Sets up the CPU timer for time base
    HAL_setupTimeBaseTimer(handle, USER_TIME_BASE_FREQ_Hz);

    // Sets up the timers for CPU usage diagnostics
    HAL_setupCPUUsageTimer(handle);

    // setup the sci
    HAL_setupSCIA(handle);

    // setup the i2c
//    HAL_setupI2CA(handle);

    // setup the DMA
    HAL_setupDMA();

    return;
} // end of HAL_setParams() function

void HAL_MTR_setParams(HAL_MTR_Handle handle, USER_Params *pUserParams)
{
    HAL_setNumCurrentSensors(handle, pUserParams->numCurrentSensors);
    HAL_setNumVoltageSensors(handle, pUserParams->numVoltageSensors);

    // setup the PWMs
    HAL_setupPWMs(handle);

    // setup the CMPSSs
    HAL_setupCMPSSs(handle);

    // setup the EQEPs
    HAL_setupQEP(handle);

    // setup the spi for drv8323/drv8353/drv8316
    HAL_setupSPI(handle);

    // setup the Gate Enable
    HAL_setupGate(handle);
    // BSXL8323RS_REVA || BSXL8353RS_REVA || BSXL8316RT_REVA

    return;
} // end of HAL_MTR_setParams() function

void HAL_setupQEP(HAL_MTR_Handle handle)
{
    HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;

	EQEP_disableInterrupt(obj->eqepHandle, EQEP_INT_GLOBAL);
	EQEP_clearInterruptStatus(obj->eqepHandle, EQEP_INT_GLOBAL);

	EQEP_SourceSelect myEqepSourceSelec;
	myEqepSourceSelec.sourceA = EQEP_SOURCE_DEVICE_PIN;
	myEqepSourceSelec.sourceB = EQEP_SOURCE_DEVICE_PIN;
	myEqepSourceSelec.sourceIndex = EQEP_SOURCE_DEVICE_PIN;
	EQEP_selectSource(obj->eqepHandle,myEqepSourceSelec);
	EQEP_setStrobeSource(obj->eqepHandle, EQEP_STROBE_FROM_GPIO);
	EQEP_setInputPolarity(obj->eqepHandle, 0, 0, 0, 0);
	EQEP_setDecoderConfig(obj->eqepHandle, EQEP_CONFIG_NO_SWAP|EQEP_CONFIG_QUADRATURE|EQEP_CONFIG_IGATE_DISABLE|EQEP_CONFIG_1X_RESOLUTION);
	EQEP_setEmulationMode(obj->eqepHandle, EQEP_EMULATIONMODE_RUNFREE);
	EQEP_setPositionCounterConfig(obj->eqepHandle, EQEP_POSITION_RESET_IDX, 4000);
	EQEP_setPosition(obj->eqepHandle, 0);
	EQEP_loadUnitTimer(obj->eqepHandle, 0);
	EQEP_disableUnitTimer(obj->eqepHandle);
	EQEP_disableWatchdog(obj->eqepHandle);
	EQEP_setLatchMode(obj->eqepHandle, EQEP_LATCH_UNIT_TIME_OUT|EQEP_LATCH_RISING_STROBE|EQEP_LATCH_RISING_INDEX);
	EQEP_setQMAModuleMode(obj->eqepHandle, EQEP_QMA_MODE_BYPASS);
	EQEP_disableDirectionChangeDuringIndex(obj->eqepHandle);
	EQEP_disableInterrupt(obj->eqepHandle, EQEP_INT_UNIT_TIME_OUT);
	EQEP_setPositionInitMode(obj->eqepHandle, 4000);
	EQEP_setSWPositionInit(obj->eqepHandle, true);
	EQEP_setInitialPosition(obj->eqepHandle, 0);
//	EQEP_enableInterrupt(obj->eqepHandle, EQEP_INT_INDEX_EVNT_LATCH);
	EQEP_enableModule(obj->eqepHandle);
	return;
}

void HAL_setupADCs(HAL_Handle handle)
{
    HAL_Obj *obj = (HAL_Obj *)handle;

    SysCtl_delay(100U);
    ASysCtl_disableTemperatureSensor();

	HWREG(ADCA_BASE + ADC_O_CTL1) |= 0x8000U;	//禁用PGA
//	ASysCtl_setAnalogReferenceInternal( ASYSCTL_VREFHIA |  ASYSCTL_VREFHIC);
	 EALLOW;
	 HWREG(ANALOGSUBSYS_BASE + ASYSCTL_O_ANAREFCTL) = 0x1;
	 EDIS;

//	DEVICE_DELAY_US(10000);

    // Set main clock scaling factor (50MHz max clock for the ADC module)
    ADC_setPrescaler(obj->adcHandle[0], ADC_CLK_DIV_3_0);
    ADC_setPrescaler(obj->adcHandle[1], ADC_CLK_DIV_3_0);

    // set the ADC interrupt pulse generation to end of conversion
    ADC_setInterruptPulseMode(obj->adcHandle[0], ADC_PULSE_END_OF_CONV);
    ADC_setInterruptPulseMode(obj->adcHandle[1], ADC_PULSE_END_OF_CONV);

    // set priority of SOCs
    ADC_setSOCPriority(obj->adcHandle[0], ADC_PRI_ALL_HIPRI);
    ADC_setSOCPriority(obj->adcHandle[1], ADC_PRI_ALL_HIPRI);

//    ADC_setSOCPriority(PFC_IAC_ADC_BASE, ADC_PRI_SOC0_HIPRI);

    // enable the ADCs
    ADC_enableConverter(obj->adcHandle[0]);
    ADC_enableConverter(obj->adcHandle[1]);

    // delay to allow ADCs to power up
    SysCtl_delay(10000U);

    //-------------------------------------------------------------------------

    // Interrupt for motor 1
    ADC_setInterruptSource(MTR1_ADC_INT_BASE,
						   MTR1_ADC_INT_NUM, ADC_SOC_NUMBER13);
//    ADC_setInterruptSource(MTR1_ADC_INT_BASE,
//                           MTR1_ADC_INT_NUM, MTR1_ADC_INT_SOC);
    // ISEN_A_M1
    ADC_setupSOC(MTR1_IU_ADC_BASE, ADC_SOC_NUMBER2, MTR1_ADC_TIGGER_SOC,
                     	 MTR1_IU_ADC_CH_NUM, MTR1_ADC_I_SAMPLEWINDOW);

    ADC_setupSOC(MTR1_IU_ADC_BASE, ADC_SOC_NUMBER3, MTR1_ADC_TIGGER_SOC,
                     	 MTR1_IU_ADC_CH_NUM, MTR1_ADC_I_SAMPLEWINDOW);

    ADC_setupSOC(MTR1_IU_ADC_BASE, MTR1_IU_ADC_SOC_NUM, MTR1_ADC_TIGGER_SOC,
                 	 	 MTR1_IU_ADC_CH_NUM, MTR1_ADC_I_SAMPLEWINDOW);

    // Configure PPB to eliminate subtraction related calculation
    // PPB is associated with ADCA_SOC0
//    ADC_setupPPB(MTR1_IU_ADC_BASE, MTR1_IU_ADC_PPB_NUM, MTR1_IU_ADC_SOC_NUM);

    // Write zero to this for now till offset calibration complete
//    ADC_setPPBCalibrationOffset(MTR1_IU_ADC_BASE, MTR1_IU_ADC_PPB_NUM, 0);

    // ISEN_B_M1
    ADC_setupSOC(MTR1_IV_ADC_BASE, ADC_SOC_NUMBER2  , MTR1_ADC_TIGGER_SOC,
                     	 MTR1_IV_ADC_CH_NUM, MTR1_ADC_I_SAMPLEWINDOW);

    ADC_setupSOC(MTR1_IV_ADC_BASE, ADC_SOC_NUMBER3  , MTR1_ADC_TIGGER_SOC,
                     	 MTR1_IV_ADC_CH_NUM, MTR1_ADC_I_SAMPLEWINDOW);

    ADC_setupSOC(MTR1_IV_ADC_BASE, MTR1_IV_ADC_SOC_NUM, MTR1_ADC_TIGGER_SOC,
                 	 	 MTR1_IV_ADC_CH_NUM, MTR1_ADC_I_SAMPLEWINDOW);

    // Configure PPB to eliminate subtraction related calculation
    // PPB is associated with ADCC_SOC0
//    ADC_setupPPB(MTR1_IV_ADC_BASE, MTR1_IV_ADC_PPB_NUM, MTR1_IV_ADC_SOC_NUM);

    // Write zero to this for now till offset calibration complete
//    ADC_setPPBCalibrationOffset(MTR1_IV_ADC_BASE, MTR1_IV_ADC_PPB_NUM, 0);

    // ISEN_C_M1
    ADC_setupSOC(MTR1_IW_ADC_BASE, ADC_SOC_NUMBER8  , MTR1_ADC_TIGGER_SOC,
				 	 	 MTR1_IW_ADC_CH_NUM, MTR1_ADC_I_SAMPLEWINDOW);

    ADC_setupSOC(MTR1_IW_ADC_BASE, ADC_SOC_NUMBER9  , MTR1_ADC_TIGGER_SOC,
				 	 	 MTR1_IW_ADC_CH_NUM, MTR1_ADC_I_SAMPLEWINDOW);

    ADC_setupSOC(MTR1_IW_ADC_BASE, ADC_SOC_NUMBER10  , MTR1_ADC_TIGGER_SOC,
				 	 	 MTR1_IW_ADC_CH_NUM, MTR1_ADC_I_SAMPLEWINDOW);

//    ADC_setupSOC(MTR1_IW_ADC_BASE, MTR1_IW_ADC_SOC_NUM, MTR1_ADC_TIGGER_SOC,
//                 MTR1_IW_ADC_CH_NUM, MTR1_ADC_I_SAMPLEWINDOW);

    // Configure PPB to eliminate subtraction related calculation
    // PPB is associated with ADCA_SOC0
//    ADC_setupPPB(MTR1_IW_ADC_BASE, MTR1_IW_ADC_PPB_NUM, MTR1_IW_ADC_SOC_NUM);

    // Write zero to this for now till offset calibration complete
//   ADC_setPPBCalibrationOffset(MTR1_IW_ADC_BASE, MTR1_IW_ADC_PPB_NUM, 0);



    // VSEN_A_M1
    ADC_setupSOC(MTR1_VU_ADC_BASE, ADC_SOC_NUMBER5  , MTR1_ADC_TIGGER_SOC,
                     	 MTR1_VU_ADC_CH_NUM, MTR1_ADC_I_SAMPLEWINDOW);

    ADC_setupSOC(MTR1_VU_ADC_BASE, ADC_SOC_NUMBER6  , MTR1_ADC_TIGGER_SOC,
                     	 MTR1_VU_ADC_CH_NUM, MTR1_ADC_I_SAMPLEWINDOW);

    ADC_setupSOC(MTR1_VU_ADC_BASE, MTR1_VU_ADC_SOC_NUM, MTR1_ADC_TIGGER_SOC,
                 	 	 MTR1_VU_ADC_CH_NUM, MTR1_ADC_I_SAMPLEWINDOW);

    // VSEN_B_M1
    ADC_setupSOC(MTR1_VV_ADC_BASE, ADC_SOC_NUMBER6, MTR1_ADC_TIGGER_SOC,
                     	 MTR1_VV_ADC_CH_NUM, MTR1_ADC_I_SAMPLEWINDOW);

    ADC_setupSOC(MTR1_VV_ADC_BASE, ADC_SOC_NUMBER7, MTR1_ADC_TIGGER_SOC,
                     	 MTR1_VV_ADC_CH_NUM, MTR1_ADC_I_SAMPLEWINDOW);

    ADC_setupSOC(MTR1_VV_ADC_BASE, MTR1_VV_ADC_SOC_NUM, MTR1_ADC_TIGGER_SOC,
                 	 	 MTR1_VV_ADC_CH_NUM, MTR1_ADC_I_SAMPLEWINDOW);

    // VSEN_C_M1
	ADC_setupSOC(MTR1_VW_ADC_BASE, ADC_SOC_NUMBER7  , MTR1_ADC_TIGGER_SOC,
				 	 	 MTR1_VW_ADC_CH_NUM, MTR1_ADC_I_SAMPLEWINDOW);

	ADC_setupSOC(MTR1_VW_ADC_BASE, ADC_SOC_NUMBER8  , MTR1_ADC_TIGGER_SOC,
				 	 	 MTR1_VW_ADC_CH_NUM, MTR1_ADC_I_SAMPLEWINDOW);

	ADC_setupSOC(MTR1_VW_ADC_BASE, ADC_SOC_NUMBER9  , MTR1_ADC_TIGGER_SOC,
				 	 	 MTR1_VW_ADC_CH_NUM, MTR1_ADC_I_SAMPLEWINDOW);

//    ADC_setupSOC(MTR1_VW_ADC_BASE, MTR1_VW_ADC_SOC_NUM, MTR1_ADC_TIGGER_SOC,
//                 MTR1_VW_ADC_CH_NUM, MTR1_ADC_I_SAMPLEWINDOW);


    // VSEN_DCBUS-->Trig Interrupt
	ADC_setupSOC(MTR1_VDC_ADC_BASE, ADC_SOC_NUMBER11 , MTR1_ADC_TIGGER_SOC,
                 	 	 MTR1_VDC_ADC_CH_NUM, MTR1_ADC_V_SAMPLEWINDOW);

	ADC_setupSOC(MTR1_VDC_ADC_BASE, ADC_SOC_NUMBER12 , MTR1_ADC_TIGGER_SOC,
                 	 	 MTR1_VDC_ADC_CH_NUM, MTR1_ADC_V_SAMPLEWINDOW);

	ADC_setupSOC(MTR1_VDC_ADC_BASE, ADC_SOC_NUMBER13 , MTR1_ADC_TIGGER_SOC,
                 	 	 MTR1_VDC_ADC_CH_NUM, MTR1_ADC_V_SAMPLEWINDOW);

//    ADC_setupSOC(MTR1_VDC_ADC_BASE, MTR1_VDC_ADC_SOC_NUM, MTR1_ADC_TIGGER_SOC,
//                 MTR1_VDC_ADC_CH_NUM, MTR1_ADC_V_SAMPLEWINDOW);

    return;
} // end of HAL_setupADCs() function


// HAL_setupCMPSSs
void HAL_setupCMPSSs(HAL_MTR_Handle handle)
{
    HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;
    // Refer to the Table 9-2 in Chapter 9 of TMS320F28004x
    // Technical Reference Manual (SPRUI33B), to configure the ePWM X-Bar
    uint16_t cmpsaDACH = MTR1_CMPSS_DACH_VALUE;
    uint16_t cmpsaDACL = MTR1_CMPSS_DACL_VALUE;

    uint16_t  cnt;

    ASysCtl_selectCMPHPMux(MTR1_IU_CMPHP_SEL, MTR1_IU_CMPHP_MUX);
    ASysCtl_selectCMPLPMux(MTR1_IU_CMPLP_SEL, MTR1_IU_CMPLP_MUX);

    ASysCtl_selectCMPHPMux(MTR1_IV_CMPHP_SEL, MTR1_IV_CMPHP_MUX);
    ASysCtl_selectCMPLPMux(MTR1_IV_CMPLP_SEL, MTR1_IV_CMPLP_MUX);

    ASysCtl_selectCMPHPMux(MTR1_IW_CMPHP_SEL, MTR1_IW_CMPHP_MUX);
    ASysCtl_selectCMPLPMux(MTR1_IW_CMPLP_SEL, MTR1_IW_CMPLP_MUX);

    for(cnt=0; cnt<3; cnt++)
    {
        // Enable CMPSS and configure the negative input signal to come from the DAC
        CMPSS_enableModule(obj->cmpssHandle[cnt]);

        // NEG signal from DAC for COMP-H
        CMPSS_configHighComparator(obj->cmpssHandle[cnt], CMPSS_INSRC_DAC);

        // NEG signal from DAC for COMP-L
        CMPSS_configLowComparator(obj->cmpssHandle[cnt], CMPSS_INSRC_DAC);

        // Configure the output signals. Both CTRIPH and CTRIPOUTH will be fed by
        // the asynchronous comparator output.
        // Dig filter output ==> CTRIPH, Dig filter output ==> CTRIPOUTH
        CMPSS_configOutputsHigh(obj->cmpssHandle[cnt],
                                CMPSS_TRIP_FILTER |
                                CMPSS_TRIPOUT_FILTER);

        // Dig filter output ==> CTRIPL, Dig filter output ==> CTRIPOUTL
        CMPSS_configOutputsLow(obj->cmpssHandle[cnt],
                               CMPSS_TRIP_FILTER |
                               CMPSS_TRIPOUT_FILTER |
                               CMPSS_INV_INVERTED);

        // Configure digital filter. For this example, the maxiumum values will be
        // used for the clock prescale, sample window size, and threshold.
        CMPSS_configFilterHigh(obj->cmpssHandle[cnt], 32, 32, 30);
        CMPSS_initFilterHigh(obj->cmpssHandle[cnt]);

        // Initialize the filter logic and start filtering
        CMPSS_configFilterLow(obj->cmpssHandle[cnt], 32, 32, 30);
        CMPSS_initFilterLow(obj->cmpssHandle[cnt]);

        // Set up COMPHYSCTL register
        // COMP hysteresis set to 2x typical value
        CMPSS_setHysteresis(obj->cmpssHandle[cnt], 1);

        // Use VDDA as the reference for the DAC and set DAC value to midpoint for
        // arbitrary reference
        CMPSS_configDAC(obj->cmpssHandle[cnt],
                   CMPSS_DACREF_VDDA | CMPSS_DACVAL_SYSCLK | CMPSS_DACSRC_SHDW);

        // Set DAC-H to allowed MAX +ve current
        CMPSS_setDACValueHigh(obj->cmpssHandle[cnt], cmpsaDACH);

        // Set DAC-L to allowed MAX -ve current
        CMPSS_setDACValueLow(obj->cmpssHandle[cnt], cmpsaDACL);

        // Clear any high comparator digital filter output latch
        CMPSS_clearFilterLatchHigh(obj->cmpssHandle[cnt]);

        // Clear any low comparator digital filter output latch
        CMPSS_clearFilterLatchLow(obj->cmpssHandle[cnt]);
    }

    return;
} // end of HAL_setupCMPSSs() function

// HAL_setupGate & HAL_enableDRV

void HAL_enableDRV(HAL_MTR_Handle handle)
{
    HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;

    DRVIC_enable(obj->drvicHandle);

    return;
}  // end of HAL_enableDRV() function

void HAL_writeDRVData(HAL_MTR_Handle handle, DRVIC_VARS_t *drvicVars)
{
    HAL_MTR_Obj  *obj = (HAL_MTR_Obj *)handle;

    DRVIC_writeData(obj->drvicHandle, drvicVars);

    return;
}  // end of HAL_writeDRVData() function


void HAL_readDRVData(HAL_MTR_Handle handle, DRVIC_VARS_t *drvicVars)
{
    HAL_MTR_Obj  *obj = (HAL_MTR_Obj *)handle;

    DRVIC_readData(obj->drvicHandle, drvicVars);

    return;
}  // end of HAL_readDRVData() function

void HAL_setupDRVSPI(HAL_MTR_Handle handle, DRVIC_VARS_t *drvicVars)
{
    HAL_MTR_Obj  *obj = (HAL_MTR_Obj *)handle;

    DRVIC_setupSPI(obj->drvicHandle, drvicVars);

    return;
}  // end of HAL_setupDRVSPI() function

void HAL_setupGate(HAL_MTR_Handle handle)
{
    HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;

    DRVIC_setSPIHandle(obj->drvicHandle, obj->spiHandle);

    DRVIC_setGPIOCSNumber(obj->drvicHandle, MTR1_DRV_SPI_CS_GPIO);
    DRVIC_setGPIOENNumber(obj->drvicHandle, MTR1_GATE_EN_GPIO);

    return;
} // HAL_setupGate() function

void HAL_setupSPI(HAL_MTR_Handle handle)
{
    HAL_MTR_Obj   *obj = (HAL_MTR_Obj *)handle;

    // Must put SPI into reset before configuring it
    SPI_disableModule(obj->spiHandle);

    // SPI configuration. Use a 500kHz SPICLK and 16-bit word size, 25MHz LSPCLK
    SPI_setConfig(obj->spiHandle, DEVICE_LSPCLK_FREQ, SPI_PROT_POL0PHA0,
                  SPI_MODE_MASTER, 1600000, 16);

    SPI_disableLoopback(obj->spiHandle);

    SPI_setEmulationMode(obj->spiHandle, SPI_EMULATION_FREE_RUN);

    SPI_enableFIFO(obj->spiHandle);
    SPI_setTxFifoTransmitDelay(obj->spiHandle, 0x10);

    SPI_clearInterruptStatus(obj->spiHandle, SPI_INT_TXFF);

    // Configuration complete. Enable the module.
    SPI_enableModule(obj->spiHandle);

    return;
}  // end of HAL_setupSPI() function
// BSXL8323RS_REVA || BSXL8353RS_REVA || BSXL8316RT_REVA



void HAL_setupMtrFaults(HAL_MTR_Handle handle)
{
    HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;
    uint16_t cnt;

    // Configure TRIP 7 to OR the High and Low trips from both
    // comparator 5, 3 & 1, clear everything first
    EALLOW;
    HWREG(XBAR_EPWM_CFG_REG_BASE + MTR1_XBAR_TRIP_ADDRL) = 0;
    HWREG(XBAR_EPWM_CFG_REG_BASE + MTR1_XBAR_TRIP_ADDRH) = 0;
    EDIS;

    // Configure TRIP7 to be CTRIP5H and CTRIP5L using the ePWM X-BAR
    XBAR_setEPWMMuxConfig(MTR1_XBAR_TRIP, MTR1_IU_XBAR_EPWM_MUX);

    // Configure TRIP7 to be CTRIP1H and CTRIP1L using the ePWM X-BAR
    XBAR_setEPWMMuxConfig(MTR1_XBAR_TRIP, MTR1_IV_XBAR_EPWM_MUX);

    // Configure TRIP7 to be CTRIP3H and CTRIP3L using the ePWM X-BAR
    XBAR_setEPWMMuxConfig(MTR1_XBAR_TRIP, MTR1_IW_XBAR_EPWM_MUX);

    // Disable all the mux first
    XBAR_disableEPWMMux(MTR1_XBAR_TRIP, 0xFFFF);

    // Enable Mux 0  OR Mux 4 to generate TRIP
    XBAR_enableEPWMMux(MTR1_XBAR_TRIP, MTR1_IU_XBAR_MUX |
                                       MTR1_IV_XBAR_MUX | MTR1_IW_XBAR_MUX);


    // configure the input x bar for TZ2 to GPIO, where Over Current is connected
    XBAR_setInputPin(INPUTXBAR_BASE, MTR1_XBAR_INPUT1, MTR1_PM_nFAULT_GPIO);
    XBAR_lockInput(INPUTXBAR_BASE, MTR1_XBAR_INPUT1);

    // Configure Trip Mechanism for the Motor control software
    // -Cycle by cycle trip on CPU halt
    // -One shot fault trip zone
    // These trips need to be repeated for EPWM1 ,2 & 3

    for(cnt=0; cnt<3; cnt++)
    {
        EPWM_enableTripZoneSignals(obj->pwmHandle[cnt], MTR1_TZ_OSHT1);

        //enable DC TRIP combinational input
        EPWM_enableDigitalCompareTripCombinationInput(obj->pwmHandle[cnt],
                                              MTR1_DCTRIPIN, EPWM_DC_TYPE_DCAH);

        EPWM_enableDigitalCompareTripCombinationInput(obj->pwmHandle[cnt],
                                              MTR1_DCTRIPIN, EPWM_DC_TYPE_DCBH);

        // Trigger event when DCAH is High
        EPWM_setTripZoneDigitalCompareEventCondition(obj->pwmHandle[cnt],
                                                     EPWM_TZ_DC_OUTPUT_A1,
                                                     EPWM_TZ_EVENT_DCXH_HIGH);

        // Trigger event when DCBH is High
        EPWM_setTripZoneDigitalCompareEventCondition(obj->pwmHandle[cnt],
                                                     EPWM_TZ_DC_OUTPUT_B1,
                                                     EPWM_TZ_EVENT_DCXL_HIGH);

        // Configure the DCA path to be un-filtered and asynchronous
        EPWM_setDigitalCompareEventSource(obj->pwmHandle[cnt],
                                          EPWM_DC_MODULE_A,
                                          EPWM_DC_EVENT_1,
                                          EPWM_DC_EVENT_SOURCE_FILT_SIGNAL);

        // Configure the DCB path to be un-filtered and asynchronous
        EPWM_setDigitalCompareEventSource(obj->pwmHandle[cnt],
                                          EPWM_DC_MODULE_B,
                                          EPWM_DC_EVENT_1,
                                          EPWM_DC_EVENT_SOURCE_FILT_SIGNAL);

        EPWM_setDigitalCompareEventSyncMode(obj->pwmHandle[cnt],
                                            EPWM_DC_MODULE_A,
                                            EPWM_DC_EVENT_1,
                                            EPWM_DC_EVENT_INPUT_NOT_SYNCED);

        EPWM_setDigitalCompareEventSyncMode(obj->pwmHandle[cnt],
                                            EPWM_DC_MODULE_B,
                                            EPWM_DC_EVENT_1,
                                            EPWM_DC_EVENT_INPUT_NOT_SYNCED);

        // Enable DCA as OST
        EPWM_enableTripZoneSignals(obj->pwmHandle[cnt], EPWM_TZ_SIGNAL_DCAEVT1);

        // Enable DCB as OST
        EPWM_enableTripZoneSignals(obj->pwmHandle[cnt], EPWM_TZ_SIGNAL_DCBEVT1);

        // What do we want the OST/CBC events to do?
        // TZA events can force EPWMxA
        // TZB events can force EPWMxB
        EPWM_setTripZoneAction(obj->pwmHandle[cnt],
                               EPWM_TZ_ACTION_EVENT_TZA,
                               EPWM_TZ_ACTION_LOW);

        EPWM_setTripZoneAction(obj->pwmHandle[cnt],
                               EPWM_TZ_ACTION_EVENT_TZB,
                               EPWM_TZ_ACTION_LOW);
    }

    for(cnt=0; cnt<3; cnt++)
    {
        // Clear any high comparator digital filter output latch
        CMPSS_clearFilterLatchHigh(obj->cmpssHandle[cnt]);

        // Clear any low comparator digital filter output latch
        CMPSS_clearFilterLatchLow(obj->cmpssHandle[cnt]);

        // Clear any spurious fault
        EPWM_clearTripZoneFlag(obj->pwmHandle[cnt], HAL_TZFLAG_INTERRUPT_ALL);
    }

    // Clear any spurious fault
    EPWM_clearTripZoneFlag(obj->pwmHandle[0], HAL_TZFLAG_INTERRUPT_ALL);
    EPWM_clearTripZoneFlag(obj->pwmHandle[1], HAL_TZFLAG_INTERRUPT_ALL);
    EPWM_clearTripZoneFlag(obj->pwmHandle[2], HAL_TZFLAG_INTERRUPT_ALL);

    return;
} // end of HAL_setupMtrFaults() function

void HAL_setupGPIOs()
{
	/******************* M1 ********************/
	// M1 EPWM Configuration
	{
		// GPIO0->EPWM1A->M1_UH*
		GPIO_setPinConfig(GPIO_0_EPWM1_A);
		GPIO_setDirectionMode(0, GPIO_DIR_MODE_OUT);
		GPIO_setPadConfig(0, GPIO_PIN_TYPE_STD);

		// GPIO1->EPWM1B->M1_UL*
		GPIO_setPinConfig(GPIO_1_EPWM1_B);
		GPIO_setDirectionMode(1, GPIO_DIR_MODE_OUT);
		GPIO_setPadConfig(1, GPIO_PIN_TYPE_STD);

		// GPIO2->EPWM2A->M1_VH*
		GPIO_setPinConfig(GPIO_2_EPWM2_A);
		GPIO_setDirectionMode(2, GPIO_DIR_MODE_OUT);
		GPIO_setPadConfig(2, GPIO_PIN_TYPE_STD);

		// GPIO3->EPWM2B->M1_VL*
		GPIO_setPinConfig(GPIO_3_EPWM2_B);
		GPIO_setDirectionMode(3, GPIO_DIR_MODE_OUT);
		GPIO_setPadConfig(3, GPIO_PIN_TYPE_STD);

		// GPIO4->EPWM3A->M1_WH*
		GPIO_setPinConfig(GPIO_4_EPWM3_A);
		GPIO_setDirectionMode(4, GPIO_DIR_MODE_OUT);
		GPIO_setPadConfig(4, GPIO_PIN_TYPE_STD);

		// GPIO15->EPWM3B->M1_WL*
		GPIO_setPinConfig(GPIO_15_EPWM3_B);
		GPIO_setDirectionMode(15, GPIO_DIR_MODE_OUT);
		GPIO_setPadConfig(15, GPIO_PIN_TYPE_STD);
	}

	// M1 DRV8323RS Configuration
	{
	    // GPIO5->Connect to GPIO5 using a jumper wire->M1_DRV_SCS
	    GPIO_setPinConfig(GPIO_5_SPIA_STE);
	    GPIO_setDirectionMode(5, GPIO_DIR_MODE_OUT);
	    GPIO_setPadConfig(5, GPIO_PIN_TYPE_STD);

	    // GPIO8->Connect to GPIO5 using a jumper wire->M1_DRV_nSCS/GAIN
	    GPIO_setPinConfig(GPIO_8_GPIO8);
	    GPIO_writePin(8, 1);
	    GPIO_setDirectionMode(8, GPIO_DIR_MODE_OUT);
	    GPIO_setPadConfig(8, GPIO_PIN_TYPE_PULLUP);

	    // GPIO09->M1_DRV_SCLK*
	    GPIO_setPinConfig(GPIO_9_SPIA_CLK);
	    GPIO_setDirectionMode(9, GPIO_DIR_MODE_OUT);
	    GPIO_setPadConfig(9, GPIO_PIN_TYPE_PULLUP);

	    // GPIO10->SPIA_SOMI->M1_DRV_SDO*
	    GPIO_setPinConfig(GPIO_10_SPIA_SOMI);
	    GPIO_setDirectionMode(10, GPIO_DIR_MODE_IN);
	    GPIO_setPadConfig(10, GPIO_PIN_TYPE_PULLUP);

	    // GPIO11->SPIA_SIMO->M1_DRV_SDI*
	    GPIO_setPinConfig(GPIO_11_SPIA_SIMO);
	    GPIO_setDirectionMode(11, GPIO_DIR_MODE_OUT);
	    GPIO_setPadConfig(11, GPIO_PIN_TYPE_PULLUP);

	    // GPIO27->M1_DRV_CAL
	    GPIO_setPinConfig(GPIO_27_GPIO27);
	    GPIO_writePin(27, 0);
	    GPIO_setDirectionMode(27, GPIO_DIR_MODE_OUT);
	    GPIO_setPadConfig(27, GPIO_PIN_TYPE_STD);

	    // GPIO29->M1_DRV_ENABLE*
	    GPIO_setPinConfig(GPIO_29_GPIO29);
	    GPIO_writePin(29, 1);
	    GPIO_setDirectionMode(29, GPIO_DIR_MODE_OUT);
	    GPIO_setPadConfig(29, GPIO_PIN_TYPE_STD);

	    // GPIO42->M1_DRV_MODE#
	    GPIO_setPinConfig(GPIO_42_GPIO42);
	    GPIO_setDirectionMode(42, GPIO_DIR_MODE_IN);
	    GPIO_setPadConfig(42, GPIO_PIN_TYPE_STD);

	    // GPIO34->M1_DRV_nFAULT*
	    GPIO_setPinConfig(GPIO_34_GPIO34);
	    GPIO_setDirectionMode(34, GPIO_DIR_MODE_IN);
	    GPIO_setPadConfig(34, GPIO_PIN_TYPE_STD);
	}

	// M1 ABS Encoder Configuration
	{
		// GPIO35->M1_ABS_DATA
		GPIO_setPinConfig(GPIO_35_SCIA_RX);
		GPIO_setPadConfig(35, GPIO_PIN_TYPE_STD | GPIO_PIN_TYPE_PULLUP);
		GPIO_setQualificationMode(35, GPIO_QUAL_ASYNC);

		// GPIO46->M1_ABS_TRIGGER
		GPIO_setDirectionMode(46, GPIO_DIR_MODE_IN);
		GPIO_setQualificationMode(46, GPIO_QUAL_SYNC);
		GPIO_setInterruptPin(46, GPIO_INT_XINT3);
		GPIO_setInterruptType(GPIO_INT_XINT3, GPIO_INT_TYPE_RISING_EDGE);
		GPIO_enableInterrupt(GPIO_INT_XINT3);
	}
	// M1 Mini Panel Encoder Configuration
	{
		// GPIO44->EQEP_A
		GPIO_setPinConfig(GPIO_44_EQEP1_A);
	    GPIO_setDirectionMode(44, GPIO_DIR_MODE_IN);
	    GPIO_setPadConfig(44, GPIO_PIN_TYPE_STD);

	    // GPIO37->EQEP_B
		GPIO_setPinConfig(GPIO_37_EQEP1_B);
	    GPIO_setDirectionMode(37, GPIO_DIR_MODE_IN);
	    GPIO_setPadConfig(37, GPIO_PIN_TYPE_STD);

	    // GPIO43->EQEP_I
		GPIO_setPinConfig(GPIO_43_EQEP1_INDEX);
	    GPIO_setDirectionMode(43, GPIO_DIR_MODE_IN);
	    GPIO_setPadConfig(43, GPIO_PIN_TYPE_STD);
	}
	// M1 Led Configuration
	{
	    // GPIO19->M1_DRV_LED
	    GPIO_setPinConfig(GPIO_19_GPIO19_X1);
	    GPIO_writePin(19, 1);
	    GPIO_setDirectionMode(19, GPIO_DIR_MODE_OUT);
	    GPIO_setPadConfig(19, GPIO_PIN_TYPE_STD);
	}

	/******************* M2 ********************/
	// M2 EPWM Configuration
	{
	    // GPIO12->EPWM7A->M2_UH*
	    GPIO_setPinConfig(GPIO_12_EPWM7_A);
	    GPIO_setDirectionMode(12, GPIO_DIR_MODE_OUT);
	    GPIO_setPadConfig(12, GPIO_PIN_TYPE_STD);

	    // GPIO13->EPWM7B->M2_UL*
	    GPIO_setPinConfig(GPIO_13_EPWM7_B);
	    GPIO_setDirectionMode(13, GPIO_DIR_MODE_OUT);
	    GPIO_setPadConfig(13, GPIO_PIN_TYPE_STD);

	    // GPIO6->EPWM4A->M2_VH*
	    GPIO_setPinConfig(GPIO_6_EPWM4_A);
	    GPIO_setDirectionMode(6, GPIO_DIR_MODE_OUT);
	    GPIO_setPadConfig(6, GPIO_PIN_TYPE_STD);

	    // GPIO7->EPWM4B->M2_VL*
	    GPIO_setPinConfig(GPIO_7_EPWM4_B);
	    GPIO_setDirectionMode(7, GPIO_DIR_MODE_OUT);
	    GPIO_setPadConfig(7, GPIO_PIN_TYPE_STD);

	    // GPIO16->EPWM5A->M2_WH*
	    GPIO_setPinConfig(GPIO_16_EPWM5_A);
	    GPIO_setDirectionMode(16, GPIO_DIR_MODE_OUT);
	    GPIO_setPadConfig(16, GPIO_PIN_TYPE_STD);

	    // GPIO17->EPWM5B->M2_WL*
	    GPIO_setPinConfig(GPIO_17_EPWM5_B);
	    GPIO_setDirectionMode(17, GPIO_DIR_MODE_OUT);
	    GPIO_setPadConfig(17, GPIO_PIN_TYPE_STD);
	}

	// M2 DRV8323RS Configuration
	{
	    // GPIO33->Connect to GPIO32 using a jumper wire->M2_DRV_nSCS/GAIN
	    GPIO_setPinConfig(GPIO_33_SPIB_STE);
	    GPIO_setDirectionMode(33, GPIO_DIR_MODE_OUT);
	    GPIO_setPadConfig(33, GPIO_PIN_TYPE_STD);

	    // GPIO32->Connect to GPIO32 using a jumper wire->M2_DRV_nSCS/GAIN
	    GPIO_setPinConfig(GPIO_32_GPIO32);
	    GPIO_writePin(32, 1);
	    GPIO_setDirectionMode(32, GPIO_DIR_MODE_OUT);
	    GPIO_setPadConfig(32, GPIO_PIN_TYPE_PULLUP);

	    // GPIO22->SPIB_CLK->M2_DRV_SCLK/DAC128S_SDI
	    GPIO_setPinConfig(GPIO_22_SPIB_CLK);
	    GPIO_setDirectionMode(22, GPIO_DIR_MODE_OUT);
	    GPIO_setPadConfig(22, GPIO_PIN_TYPE_PULLUP);

	    // GPIO31->SPIB_SOMI->DAC128S_SDO*
	    GPIO_setPinConfig(GPIO_31_SPIB_SOMI);
	    GPIO_setDirectionMode(31, GPIO_DIR_MODE_IN);
	    GPIO_setPadConfig(31, GPIO_PIN_TYPE_PULLUP);

	    // GPIO30->SPIB_SIMO->DAC128S_SDI*
	    GPIO_setPinConfig(GPIO_30_SPIB_SIMO);
	    GPIO_setDirectionMode(30, GPIO_DIR_MODE_OUT);
	    GPIO_setPadConfig(30, GPIO_PIN_TYPE_PULLUP);

	    // GPIO24->M2_DRV_ENABLE*
	    GPIO_setPinConfig(GPIO_24_GPIO24);
	    GPIO_writePin(24, 1);
	    GPIO_setDirectionMode(24, GPIO_DIR_MODE_OUT);
	    GPIO_setPadConfig(24, GPIO_PIN_TYPE_STD);

	    // GPIO39->M2_DRV_nFAULT*
	    GPIO_setPinConfig(GPIO_41_GPIO41);
	    GPIO_setDirectionMode(41, GPIO_DIR_MODE_IN);
	    GPIO_setPadConfig(41, GPIO_PIN_TYPE_STD);
	}

	// M2 ABS Encoder Configuration
	{

	}

	// M2 Mini Panel Encoder Configuration
	{
		// GPIO14->EQEP_A
		GPIO_setPinConfig(GPIO_14_EQEP2_A);
	    GPIO_setDirectionMode(14, GPIO_DIR_MODE_IN);
	    GPIO_setPadConfig(14, GPIO_PIN_TYPE_STD);

	    // GPIO25->EQEP_B
		GPIO_setPinConfig(GPIO_25_EQEP2_B);
	    GPIO_setDirectionMode(25, GPIO_DIR_MODE_IN);
	    GPIO_setPadConfig(25, GPIO_PIN_TYPE_STD);

	    // GPIO26->EQEP_I
		GPIO_setPinConfig(GPIO_26_EQEP2_INDEX);
	    GPIO_setDirectionMode(26, GPIO_DIR_MODE_IN);
	    GPIO_setPadConfig(26, GPIO_PIN_TYPE_STD);
	}

	// M2 Led Configuration
	{
	    // GPIO18->Reserve
	    GPIO_setPinConfig(GPIO_18_GPIO18_X2);
	    GPIO_setDirectionMode(18, GPIO_DIR_MODE_IN);
	    GPIO_setPadConfig(18, GPIO_PIN_TYPE_STD);
	}

    return;
}  // end of HAL_setupGPIOs() function

void HAL_setupPeripheralClks()
{
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_DMA);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TIMER0);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TIMER1);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TIMER2);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_HRPWM);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM1);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM2);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM3);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM4);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM5);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM6);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM7);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ECAP1);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ECAP2);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ECAP3);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EQEP1);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EQEP2);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SCIA);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SPIA);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SPIB);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_I2CA);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CANA);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ADCA);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ADCC);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CMPSS1);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CMPSS2);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CMPSS3);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CMPSS4);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_FSITXA);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_FSIRXA);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_LINA);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_LINB);
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_PMBUSA);

    return;
} // end of HAL_setupPeripheralClks() function

void HAL_setupPWMs(HAL_MTR_Handle handle)
{
    HAL_MTR_Obj    *obj = (HAL_MTR_Obj *)handle;
    uint16_t  cnt;

    uint16_t       pwmPeriodCycles = (uint16_t)(USER_M1_PWM_TBPRD_NUM);
    uint16_t       numPWMTicksPerISRTick = USER_M1_NUM_PWM_TICKS_PER_ISR_TICK;

    // disable the ePWM module time base clock sync signal
    // to synchronize all of the PWMs
    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    // turns off the outputs of the EPWM peripherals which will put the power
    // switches into a high impedance state.
    EPWM_forceTripZoneEvent(obj->pwmHandle[0], EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(obj->pwmHandle[1], EPWM_TZ_FORCE_EVENT_OST);
    EPWM_forceTripZoneEvent(obj->pwmHandle[2], EPWM_TZ_FORCE_EVENT_OST);

    for(cnt=0; cnt<3; cnt++)
    {
        // setup the Time-Base Control Register (TBCTL)
        EPWM_setTimeBaseCounterMode(obj->pwmHandle[cnt],
                                    EPWM_COUNTER_MODE_UP_DOWN);

        EPWM_disablePhaseShiftLoad(obj->pwmHandle[cnt]);

        EPWM_setPeriodLoadMode(obj->pwmHandle[cnt], EPWM_PERIOD_DIRECT_LOAD);

        EPWM_enableSyncOutPulseSource(obj->pwmHandle[cnt],
                                      EPWM_SYNC_OUT_PULSE_ON_SOFTWARE);

        EPWM_setClockPrescaler(obj->pwmHandle[cnt], EPWM_CLOCK_DIVIDER_1,
                                 EPWM_HSCLOCK_DIVIDER_1);

        EPWM_setCountModeAfterSync(obj->pwmHandle[cnt],
                                   EPWM_COUNT_MODE_UP_AFTER_SYNC);

        EPWM_setEmulationMode(obj->pwmHandle[cnt], EPWM_EMULATION_FREE_RUN);

        // setup the Timer-Based Phase Register (TBPHS)
        EPWM_setPhaseShift(obj->pwmHandle[cnt], 0);

        // setup the Time-Base Counter Register (TBCTR)
        EPWM_setTimeBaseCounter(obj->pwmHandle[cnt], 0);

        // setup the Time-Base Period Register (TBPRD)
        // set to zero initially
        EPWM_setTimeBasePeriod(obj->pwmHandle[cnt], 0);

        // setup the Counter-Compare Control Register (CMPCTL)
        EPWM_setCounterCompareShadowLoadMode(obj->pwmHandle[cnt],
                                             EPWM_COUNTER_COMPARE_A,
                                             EPWM_COMP_LOAD_ON_CNTR_ZERO);

        EPWM_setCounterCompareShadowLoadMode(obj->pwmHandle[cnt],
                                             EPWM_COUNTER_COMPARE_B,
                                             EPWM_COMP_LOAD_ON_CNTR_ZERO);

        EPWM_setCounterCompareShadowLoadMode(obj->pwmHandle[cnt],
                                             EPWM_COUNTER_COMPARE_C,
                                             EPWM_COMP_LOAD_ON_CNTR_ZERO);

        EPWM_setCounterCompareShadowLoadMode(obj->pwmHandle[cnt],
                                             EPWM_COUNTER_COMPARE_D,
                                             EPWM_COMP_LOAD_ON_CNTR_ZERO);


        // setup the Action-Qualifier Output A Register (AQCTLA)
        EPWM_setActionQualifierAction(obj->pwmHandle[cnt],
                                      EPWM_AQ_OUTPUT_A,
                                      EPWM_AQ_OUTPUT_HIGH,
                                      EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);

        EPWM_setActionQualifierAction(obj->pwmHandle[cnt],
                                      EPWM_AQ_OUTPUT_A,
                                      EPWM_AQ_OUTPUT_HIGH,
                                      EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);

        EPWM_setActionQualifierAction(obj->pwmHandle[cnt],
                                      EPWM_AQ_OUTPUT_A,
                                      EPWM_AQ_OUTPUT_LOW,
                                      EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);

        EPWM_setActionQualifierAction(obj->pwmHandle[cnt],
                                      EPWM_AQ_OUTPUT_A,
                                      EPWM_AQ_OUTPUT_LOW,
                                      EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);


        // setup the Dead-Band Generator Control Register (DBCTL)
        EPWM_setDeadBandDelayMode(obj->pwmHandle[cnt], EPWM_DB_RED, true);
        EPWM_setDeadBandDelayMode(obj->pwmHandle[cnt], EPWM_DB_FED, true);

        // select EPWMA as the input to the dead band generator
        EPWM_setRisingEdgeDeadBandDelayInput(obj->pwmHandle[cnt],
                                             EPWM_DB_INPUT_EPWMA);

        // configure the right polarity for active high complementary config.
        EPWM_setDeadBandDelayPolarity(obj->pwmHandle[cnt],
                                      EPWM_DB_RED,
                                      EPWM_DB_POLARITY_ACTIVE_HIGH);
        EPWM_setDeadBandDelayPolarity(obj->pwmHandle[cnt],
                                      EPWM_DB_FED,
                                      EPWM_DB_POLARITY_ACTIVE_LOW);

        // setup the Dead-Band Rising Edge Delay Register (DBRED)
        EPWM_setRisingEdgeDelayCount(obj->pwmHandle[cnt], MTR1_PWM_DBRED_CNT);

        // setup the Dead-Band Falling Edge Delay Register (DBFED)
        EPWM_setFallingEdgeDelayCount(obj->pwmHandle[cnt], MTR1_PWM_DBFED_CNT);



        // setup the PWM-Chopper Control Register (PCCTL)
        EPWM_disableChopper(obj->pwmHandle[cnt]);

        // setup the Trip Zone Select Register (TZSEL)
        EPWM_disableTripZoneSignals(obj->pwmHandle[cnt], HAL_TZSEL_SIGNALS_ALL);
    }

    // BSXL8323RS_REVA || BSXL8323RH_REVB || \
    // BSXL8353RS_REVA || BSXL8316RT_REVA || \
    // BSXL3PHGAN_REVA || HVMTRPFC_REV1P1
    // setup the Event Trigger Selection Register (ETSEL)
    EPWM_setInterruptSource(obj->pwmHandle[0], EPWM_INT_TBCTR_ZERO);

    EPWM_enableInterrupt(obj->pwmHandle[0]);

    EPWM_setADCTriggerSource(obj->pwmHandle[0],
                             EPWM_SOC_A, EPWM_SOC_TBCTR_D_CMPC);

    EPWM_enableADCTrigger(obj->pwmHandle[0], EPWM_SOC_A);


    // setup the Event Trigger Prescale Register (ETPS)
    if(numPWMTicksPerISRTick > 15)
    {
        EPWM_setInterruptEventCount(obj->pwmHandle[0], 15);

        EPWM_setADCTriggerEventPrescale(obj->pwmHandle[0], EPWM_SOC_A, 15);
        EPWM_setADCTriggerEventPrescale(obj->pwmHandle[0], EPWM_SOC_B, 15);

    }
    else if(numPWMTicksPerISRTick < 1)
    {
        EPWM_setInterruptEventCount(obj->pwmHandle[0], 1);

        EPWM_setADCTriggerEventPrescale(obj->pwmHandle[0], EPWM_SOC_A, 1);
        EPWM_setADCTriggerEventPrescale(obj->pwmHandle[0], EPWM_SOC_B, 1);

    }
    else
    {
        EPWM_setInterruptEventCount(obj->pwmHandle[0], numPWMTicksPerISRTick);

        EPWM_setADCTriggerEventPrescale(obj->pwmHandle[0], EPWM_SOC_A,
                                        numPWMTicksPerISRTick);
        EPWM_setADCTriggerEventPrescale(obj->pwmHandle[0], EPWM_SOC_B,
                                        numPWMTicksPerISRTick);

    }

    // setup the Event Trigger Clear Register (ETCLR)
    EPWM_clearEventTriggerInterruptFlag(obj->pwmHandle[0]);
    EPWM_clearADCTriggerFlag(obj->pwmHandle[0], EPWM_SOC_A);
    EPWM_clearADCTriggerFlag(obj->pwmHandle[0], EPWM_SOC_B);

    // since the PWM is configured as an up/down counter, the period register is
    // set to one-half of the desired PWM period
    EPWM_setTimeBasePeriod(obj->pwmHandle[0], pwmPeriodCycles);
    EPWM_setTimeBasePeriod(obj->pwmHandle[1], pwmPeriodCycles);
    EPWM_setTimeBasePeriod(obj->pwmHandle[2], pwmPeriodCycles);

    // write the PWM data value  for ADC trigger
    EPWM_setCounterCompareValue(obj->pwmHandle[0], EPWM_COUNTER_COMPARE_C, 10);


    // enable the ePWM module time base clock sync signal
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    return;
}  // end of HAL_setupPWMs() function


void HAL_setupSCIA(HAL_Handle halHandle)
{
    HAL_Obj *obj = (HAL_Obj *)halHandle;

    // Initialize SCIA and its FIFO.
	SCI_clearInterruptStatus(obj->sciHandle, SCI_INT_RXFF | SCI_INT_TXFF | SCI_INT_FE | SCI_INT_OE | SCI_INT_PE | SCI_INT_RXERR | SCI_INT_RXRDY_BRKDT | SCI_INT_TXRDY);
	SCI_clearOverflowStatus(obj->sciHandle);
	SCI_resetTxFIFO(obj->sciHandle);
	SCI_resetRxFIFO(obj->sciHandle);
	SCI_resetChannels(obj->sciHandle);
	SCI_setConfig(obj->sciHandle, DEVICE_LSPCLK_FREQ, 921600, (SCI_CONFIG_WLEN_8|SCI_CONFIG_STOP_ONE|SCI_CONFIG_PAR_NONE));
	SCI_disableLoopback(obj->sciHandle);
	SCI_performSoftwareReset(obj->sciHandle);
	SCI_enableFIFO(obj->sciHandle);
	SCI_setFIFOInterruptLevel(obj->sciHandle,SCI_FIFO_TX0,SCI_FIFO_RX5);
	SCI_enableModule(obj->sciHandle);

    return;
}  // end of DRV_setupSci() function


void HAL_setupTimeBaseTimer(HAL_Handle handle, const float32_t timeBaseFreq_Hz)
{
    HAL_Obj  *obj = (HAL_Obj *)handle;

    uint32_t timerPeriod = (uint32_t)((USER_SYSTEM_FREQ_MHz * 1000.0f *1000.0f) /
                                      timeBaseFreq_Hz);

    // use timer 0 for CPU usage diagnostics
    CPUTimer_setPreScaler(obj->timerHandle[0], 0);

    CPUTimer_setEmulationMode(obj->timerHandle[0],
                              CPUTIMER_EMULATIONMODE_RUNFREE);

    CPUTimer_setPeriod(obj->timerHandle[0], timerPeriod);

    CPUTimer_startTimer(obj->timerHandle[0]);

    return;
}  // end of HAL_setupTimeBaseTimer() function


void HAL_setupADCTriggerTimer(HAL_Handle handle, const float32_t adcTriggerFreq_Hz)
{
    HAL_Obj  *obj = (HAL_Obj *)handle;

    uint32_t timerPeriod = (uint32_t)((USER_SYSTEM_FREQ_MHz * 1000.0f *1000.0f) /
                                      adcTriggerFreq_Hz);

    // use timer 1 for CPU usage diagnostics
    CPUTimer_setPreScaler(obj->timerHandle[1], 0);

    CPUTimer_setEmulationMode(obj->timerHandle[1],
                              CPUTIMER_EMULATIONMODE_RUNFREE);

    CPUTimer_setPeriod(obj->timerHandle[1], timerPeriod);

    CPUTimer_enableInterrupt(obj->timerHandle[1]);

    CPUTimer_startTimer(obj->timerHandle[1]);

    return;
}  // end of HAL_setupADCTriggerTimer() function


void HAL_setupCPUUsageTimer(HAL_Handle handle)
{
    HAL_Obj  *obj = (HAL_Obj *)handle;

    // use timer 2 for CPU usage diagnostics
    CPUTimer_setPreScaler(obj->timerHandle[2], 0);

    CPUTimer_setEmulationMode(obj->timerHandle[2],
                              CPUTIMER_EMULATIONMODE_RUNFREE);

    CPUTimer_setPeriod(obj->timerHandle[2], 0xFFFFFFFF);

    CPUTimer_startTimer(obj->timerHandle[2]);

    return;
}  // end of HAL_setupCPUUsageTimer() function



void HAL_clearDataRAM(void *pMemory, uint16_t lengthMemory)
{
    uint16_t *pMemoryStart;
    uint16_t loopCount, loopLength;

    pMemoryStart = pMemory;
    loopLength = lengthMemory;

    for(loopCount = 0; loopCount < loopLength; loopCount++)
    {
        *(pMemoryStart + loopCount) = 0x0000;
    }
}   //end of HAL_clearDataRAM() function


void HAL_setupDMA(void)
{
    // Initializes the DMA controller to a known state
    DMA_initController();

    // Sets DMA emulation mode, Continue DMA operation
    DMA_setEmulationMode(DMA_EMULATION_FREE_RUN);

    return;
}    //end of HAL_setupDMA() function


void HAL_setMtrCMPSSDACValue(HAL_MTR_Handle handle,
                             const uint16_t dacValH, const uint16_t dacValL)
{
    HAL_MTR_Obj *obj = (HAL_MTR_Obj *)handle;


    CMPSS_setDACValueHigh(obj->cmpssHandle[0], dacValH);
    CMPSS_setDACValueLow(obj->cmpssHandle[0], dacValL);

    CMPSS_setDACValueHigh(obj->cmpssHandle[1], dacValH);
    CMPSS_setDACValueLow(obj->cmpssHandle[1], dacValL);

    CMPSS_setDACValueHigh(obj->cmpssHandle[2], dacValH);
    CMPSS_setDACValueLow(obj->cmpssHandle[2], dacValL);
    return;
}


void HAL_setTriggerPrams(HAL_PWMData_t *pPWMData, const float32_t systemFreq_MHz,
                   const float32_t deadband_us, const float32_t noiseWindow_us)
{
    uint16_t deadband =  (uint16_t)(deadband_us * systemFreq_MHz);
    uint16_t noiseWindow =  (uint16_t)(noiseWindow_us * systemFreq_MHz);

    pPWMData->deadband = deadband;
    pPWMData->noiseWindow = noiseWindow;

    pPWMData->minCMPValue = deadband + noiseWindow + 33;

    return;
}

// end of file
