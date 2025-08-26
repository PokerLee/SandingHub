//#############################################################################
//
// FILE:   device.c
//
// TITLE:  Device setup for examples.
//
//#############################################################################

//
// Included Files
//
#include "device.h"
#include "driverlib.h"
#ifdef __cplusplus
using std::memcpy;
#endif

//*****************************************************************************
//
// Function to initialize the device. Primarily initializes system control to a
// known state by disabling the watchdog, setting up the SYSCLKOUT frequency,
// and enabling the clocks to the peripherals.
//
//*****************************************************************************
void Device_init(void)
{
    //
    // Disable the watchdog
    //
    SysCtl_disableWatchdog();

    //
    // Call Device_cal function when run using debugger
    // This function is called as part of the Boot code. The function is called
    // in the Device_init function since during debug time resets, the boot code
    // will not be executed and the gel script will reinitialize all the
    // registers and the calibrated values will be lost.
    // Sysctl_deviceCal is a wrapper function for PMUTrims_cal OscTrims_cal
    //
    SysCtl_deviceCal();

    //
    // Call Flash Initialization to setup flash waitstates. This function must
    // reside in RAM.
    //
    Flash_initModule(FLASH0CTRL_BASE, FLASH0ECC_BASE, DEVICE_FLASH_WAITSTATES);

    //
    // Set up PLL control and clock dividers
    //
    SysCtl_setClock(DEVICE_SETCLOCK_CFG);

    //
    // Make sure the LSPCLK divider is set to the default (divide by 4)
    //
    SysCtl_setLowSpeedClock(SYSCTL_LSPCLK_PRESCALE_4);

    //
    // These asserts will check that the #defines for the clock rates in
    // device.h match the actual rates that have been configured. If they do
    // not match, check that the calculations of DEVICE_SYSCLK_FREQ and
    // DEVICE_LSPCLK_FREQ are accurate. Some examples will not perform as
    // expected if these are not correct.
    //
    ASSERT(SysCtl_getClock(DEVICE_OSCSRC_FREQ) == DEVICE_SYSCLK_FREQ);
    ASSERT(SysCtl_getLowSpeedClock(DEVICE_OSCSRC_FREQ) == DEVICE_LSPCLK_FREQ);

    //
    // Turn on all peripherals
    //
    Device_enableAllPeripherals();

    //
    // Lock VREGCTL Register
    // The register VREGCTL is not supported in this device. It is locked to
    // prevent any writes to this register
    //
    ASysCtl_lockVREG();
}

//*****************************************************************************
//
// Function to turn on all peripherals, enabling reads and writes to the
// peripherals' registers.
//
// Note that to reduce power, unused peripherals should be disabled.
//
//*****************************************************************************
void Device_enableAllPeripherals(void)
{
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_DMA);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TIMER0);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TIMER1);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TIMER2);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CPUBGCRC);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_HRPWM);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ERAD);

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM1);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM2);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM3);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM4);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM5);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM6);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM7);
	SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM8);

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ECAP1);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ECAP2);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ECAP3);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_HRCAP);

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EQEP1);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EQEP2);

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SCIA);

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SPIA);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SPIB);

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_I2CA);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_I2CB);

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_I2SA);

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CANA);

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ADCA);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ADCC);

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CMPSS1);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CMPSS2);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CMPSS3);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CMPSS4);

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_FSITXA);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_FSIRXA);

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_LINA);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_LINB);

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_PMBUSA);

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_DCC0);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_DCC1);

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CLB1);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CLB2);

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_HICA);
}

//*****************************************************************************
//
// Function to disable pin locks and enable pullups on GPIOs.
//
//*****************************************************************************
void Device_initGPIO(void)
{
    //
    // Disable pin locks.
    //
    GPIO_unlockPortConfig(GPIO_PORT_A, 0xFFFFFFFF);
    GPIO_unlockPortConfig(GPIO_PORT_B, 0xFFFFFFFF);
    GPIO_unlockPortConfig(GPIO_PORT_H, 0xFFFFFFFF);
}

//*****************************************************************************
//
// Function to verify the XTAL frequency
// freq is the XTAL frequency in MHz
// The function return true if the the actual XTAL frequency matches with the
// input value
//
// Note that this function assumes that the PLL is not already configured and
// hence uses SysClk freq = 10MHz for DCC calculation
//
//*****************************************************************************
bool Device_verifyXTAL(float freq)
{
    //
    // Use DCC to verify the XTAL frequency using INTOSC2 as reference clock
    //

    //
    // Turn on XTAL and wait for it to power up using X1CNT
    //
    SysCtl_turnOnOsc(SYSCTL_OSCSRC_XTAL);
    SysCtl_clearExternalOscCounterValue();
    while(SysCtl_getExternalOscCounterValue() != SYSCTL_X1CNT_X1CNT_M);

    //
    // Enable DCC0 clock
    //
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_DCC0);

    //
    // Configures XTAL as CLKSRC0 and INTOSC2 as CLKSRC1
    // Fclk0 = XTAL frequency (input parameter)
    // Fclk1 = INTOSC2 frequency = 10MHz
    //
    // Configuring OSC2 error tolerance of +/-10%
    // INTOSC2 can have a variance in frequency of +/-10%
    //
    // Assuming PLL is not already configured, SysClk freq = 10MHz
    //
    // Note : Update the tolerance and INTOSC2 frequency variance as necessary.
    //
    return (DCC_verifyClockFrequency(DCC0_BASE,
                                     DCC_COUNT1SRC_INTOSC2, 10.0F,
                                     DCC_COUNT0SRC_XTAL, freq,
                                     10.0F, 10.0F, 10.0F));

}

//*****************************************************************************
//
// Function to verify the XTAL frequency stability
// The function return true if the the actual XTAL frequency stability status
//
// Note that this function assumes that the PLL is not already configured and
// hence uses SysClk freq = 10MHz for DCC calculation
//
//*****************************************************************************
void Device_verifyXTALFreqStability(void)
{
	Uint32 freq = 0;
	Uint32 loopIndex = 0;
	Uint32 result[6] = {0};
	Uint32 min = 0xFFFFFFFF;
	Uint32 max = 0;
	Uint32 errorFlag = 0;

    //
    // Use DCC to verify the XTAL frequency using INTOSC2 as reference clock
    //

    //
    // Enable DCC0 clock
    //
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_DCC0);

    //
	// Using DCC test 6 times
	//
    for(loopIndex = 0; loopIndex < 6; loopIndex++)
    {
		//
		// Reset DCC0 clock
		//
		SysCtl_resetPeripheral(SYSCTL_PERIPH_RES_DCC0);

		//
		// Clear DCC error flag
		//
		DCC_clearErrorFlag(DCC0_BASE);

		//
		// Clear DCC done flag
		//
		DCC_clearDoneFlag(DCC0_BASE);

		//
		// Set INTOSC2 as counter1 clock source
		//
		DCC_setCounter1ClkSource(DCC0_BASE, DCC_COUNT1SRC_INTOSC2);

		//
		// Set XTAL as counter0 clock source
		//
		DCC_setCounter0ClkSource(DCC0_BASE, DCC_COUNT0SRC_XTAL);

		//
		// Configure DCC counter seed and valid value
		//
		EALLOW;
		HWREG(DCC0_BASE + DCC_O_CNTSEED0) = 0xFFFFF;
		HWREG(DCC0_BASE + DCC_O_CNTSEED1) = 500;
		HWREG(DCC0_BASE + DCC_O_VALIDSEED0) = 0;
		EDIS;

		//
		// Enable one shot test
		//
		DCC_enableSingleShotMode(DCC0_BASE, DCC_MODE_COUNTER_ZERO);

		//
		// Enable DCC test
		//
		DCC_enableModule(DCC0_BASE);

		//
		// Wait for test finish
		//
		while((HWREG(DCC0_BASE + DCC_O_STATUS) &
						   (DCC_STATUS_ERR | DCC_STATUS_DONE)) == 0U);

		//
		// Get counter0 value
		//
		result[loopIndex] = 0xFFFFF - DCC_getCounter0Value(DCC0_BASE);
    }
	
	//
	// Reset DCC0 clock
	//
	SysCtl_resetPeripheral(SYSCTL_PERIPH_RES_DCC0);

    //
	// Update min and max value
	//
    for(loopIndex = 0; loopIndex < 6; loopIndex++)
	{
    	//
		// Update min value
		//
		if(result[loopIndex] < min)
		{
			min = result[loopIndex];
		}

		//
		// Update max value
		//
		if(result[loopIndex] > max)
		{
			max = result[loopIndex];
		}
	}

    //
	// Check invalid counter
	//
    if(0 == min)
	{
		errorFlag = 1;
	}
	else
	{
		//
		// Check error rate
		//
		if((max - min) > (min * 6 / 100))
		{
			errorFlag = 1;
		}

		//
		// Calculate integer frequency
		//
		freq = min * 10 / 500;

		//
		// Check freqency range
		//
		if((freq < 9) || (freq > 25))
		{
			errorFlag = 1;
		}
	}

    //
	// Check result
	//
    if(0 != errorFlag)
	{
		ESTOP0;
	}
}

//*****************************************************************************
//
// Error handling function to be called when an ASSERT is violated
//
//*****************************************************************************
void __error__(char *filename, uint32_t line)
{
    //
    // An ASSERT condition was evaluated as false. You can use the filename and
    // line parameters to determine what went wrong.
    //
	filename = (char *)filename;
	line = (uint32_t)line;
    ESTOP0;
}
