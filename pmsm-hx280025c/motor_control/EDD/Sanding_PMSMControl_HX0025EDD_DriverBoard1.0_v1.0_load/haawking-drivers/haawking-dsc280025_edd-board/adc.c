//###########################################################################
//
// FILE:   adc.c
//
// TITLE:  H28x ADC driver.
//
//###########################################################################


#include "adc.h"


//*****************************************************************************
//
// ADC_setVREF
//
//*****************************************************************************
void
ADC_setVREF(uint32_t base, ADC_ReferenceMode refMode,
            ADC_ReferenceVoltage refVoltage)
{
    //
    // Check the arguments.
    //
    ASSERT(ADC_isBaseValid(base));

    EALLOW;
    //
    // Configure the reference mode (internal or external).
    //
    if((refMode == ADC_REFERENCE_INTERNAL)&&(refVoltage == ADC_REFERENCE_2_5V))
    {
    	HWREG(ANALOGSUBSYS_BASE + ASYSCTL_O_ANAREFCTL) &= ~(ASYSCTL_ANAREFCTL_ANAVREFHIEN \
    																		| ASYSCTL_ANAREFCTL_ANAREFAVDDSEL \
																			| ASYSCTL_ANAREFCTL_ANAREFCVDDSEL);
		HWREG(0x38F20) &= ~(0x8);
    }
    else if((refMode == ADC_REFERENCE_INTERNAL)&&(refVoltage == ADC_REFERENCE_VDDA))
    {
    	HWREG(ANALOGSUBSYS_BASE + ASYSCTL_O_ANAREFCTL) = (HWREG(ANALOGSUBSYS_BASE + ASYSCTL_O_ANAREFCTL) \
																		& ~ASYSCTL_ANAREFCTL_ANAREFCVDDSEL) \
																		| (ASYSCTL_ANAREFCTL_ANAREFAVDDSEL \
																		| ASYSCTL_ANAREFCTL_ANAVREFHIEN);
		HWREG(0x38F20) &= ~(0x8);
    }
    else if(refMode == ADC_REFERENCE_EXTERNAL)
    {
    	HWREG(ANALOGSUBSYS_BASE + ASYSCTL_O_ANAREFCTL) = (HWREG(ANALOGSUBSYS_BASE + ASYSCTL_O_ANAREFCTL) \
																		& ~(ASYSCTL_ANAREFCTL_ANAREFAVDDSEL \
																				| ASYSCTL_ANAREFCTL_ANAREFCVDDSEL)) \
																		| ASYSCTL_ANAREFCTL_ANAVREFHIEN;
    	HWREG(0x38F20) &= ~(0x8);
    }
    else
    {

    }

    EDIS;
}

//*****************************************************************************
//
// ADC_setPPBTripLimits
//
//*****************************************************************************
void
ADC_setPPBTripLimits(uint32_t base, ADC_PPBNumber ppbNumber,
                     int32_t tripHiLimit, int32_t tripLoLimit)
{
    uint32_t ppbHiOffset;
    uint32_t ppbLoOffset;

    //
    // Check the arguments.
    //
    ASSERT(ADC_isBaseValid(base));
    ASSERT((tripHiLimit <= 65535) && (tripHiLimit >= -65536));
    ASSERT((tripLoLimit <= 65535) && (tripLoLimit >= -65536));

    //
    // Get the offset to the appropriate trip limit registers.
    //
    ppbHiOffset = (ADC_PPBxTRIPHI_STEP * (uint32_t)ppbNumber) +
                  ADC_O_PPB1TRIPHI;
    ppbLoOffset = (ADC_PPBxTRIPLO_STEP * (uint32_t)ppbNumber) +
                  ADC_O_PPB1TRIPLO;

    EALLOW;

    //
    // Set the trip high limit.
    //
    HWREG(base + ppbHiOffset) =
        (HWREG(base + ppbHiOffset) & ~ADC_PPBTRIP_MASK) |
        ((uint32_t)tripHiLimit & ADC_PPBTRIP_MASK);

    //
    // Set the trip low limit.
    //
    HWREG(base + ppbLoOffset) =
        (HWREG(base + ppbLoOffset) & ~ADC_PPBTRIP_MASK) |
        ((uint32_t)tripLoLimit & ADC_PPBTRIP_MASK);

    EDIS;
}
