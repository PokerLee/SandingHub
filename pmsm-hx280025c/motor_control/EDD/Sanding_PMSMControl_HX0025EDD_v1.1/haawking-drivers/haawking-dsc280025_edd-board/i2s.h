//###########################################################################
//
// FILE:   i2s.h
//
// TITLE:  H28x I2S driver.
//
//###########################################################################

#ifndef I2S_H
#define I2S_H

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
//! \addtogroup i2c_api I2C
//! @{
//
//*****************************************************************************

#include "inc/hw_i2s.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "cpu.h"
#include "debug.h"


//*****************************************************************************
//
//! Values that can be passed to I2S_setDataFormat() as the \e dataFormat parameter.
//
//*****************************************************************************
typedef enum
{
    I2S_DATA_FORMAT_I2S_LJ     	= 0U,   //!<
	I2S_DATA_FORMAT_DSP      	= 1U,   //!<
} I2S_DataFormat;

//*****************************************************************************
//
//! Values that can be passed to I2S_setWordLength() as the \e wordLength parameter.
//
//*****************************************************************************
typedef enum
{
    I2S_WORD_LENGTH_8BIT      	= 0U,   //!<
	I2S_WORD_LENGTH_10BIT      	= 1U,   //!<
	I2S_WORD_LENGTH_12BIT      	= 2U,   //!<
	I2S_WORD_LENGTH_14BIT      	= 3U,   //!<
	I2S_WORD_LENGTH_16BIT      	= 4U,   //!<
	I2S_WORD_LENGTH_18BIT      	= 5U,   //!<
	I2S_WORD_LENGTH_20BIT      	= 6U,   //!<
	I2S_WORD_LENGTH_24BIT      	= 7U,   //!<
	I2S_WORD_LENGTH_32BIT      	= 8U,   //!<
} I2S_WordLength;

//*****************************************************************************
//
//! Values that can be passed to I2S_setDataDelay() as the \e dataDelay parameter.
//
//*****************************************************************************
typedef enum
{
    I2S_DATA_DELAY_0BIT      	= 0U,   //!<
	I2S_DATA_DELAY_1BIT      	= 1U,   //!<
	I2S_DATA_DELAY_2BIT      	= 2U,   //!<
} I2S_DataDelay;

//*****************************************************************************
//
//! Values that can be passed to I2S_setFSDivider() as the \e divider parameter.
//
//*****************************************************************************
typedef enum
{
    I2S_FS_DIVIDE_BY_8      	= 0U,   //!<
	I2S_FS_DIVIDE_BY_16      	= 1U,   //!<
	I2S_FS_DIVIDE_BY_32      	= 2U,   //!<
	I2S_FS_DIVIDE_BY_64      	= 3U,   //!<
	I2S_FS_DIVIDE_BY_128      	= 4U,   //!<
	I2S_FS_DIVIDE_BY_256      	= 5U,   //!<
} I2S_FSDivider;

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************
//*****************************************************************************
//
//! \internal
//! Checks an I2S base address.
//!
//! \param base is the base address of the I2S instance used.
//!
//! This function determines if a I2S module base address is valid.
//!
//! \return Returns \b true if the base address is valid and \b false
//! otherwise.
//
//*****************************************************************************
#ifdef DEBUG
static __always_inline bool
I2S_isBaseValid(uint32_t base)
{
    return(
           (base == I2SA_BASE)
          );
}
#endif

//*****************************************************************************
//
//! Set the data format.
//!
//! \param base is the base address of the I2S instance used.
//! \param dataFormat for selection of data format.
//!
//! This function is only valid when the I2S module specified by the \b base
//! parameter is a controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_setDataFormat(uint32_t base, I2S_DataFormat dataFormat)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    EALLOW;

    //
    // Enable the I2S data delay.
    //
    HWREG(base + I2S_O_SCTRL) = (HWREG(base + I2S_O_SCTRL) & ~I2S_SCTRL_FRMT_M) |\
    		(dataFormat << I2S_SCTRL_FRMT_S);

    EDIS;
}

//*****************************************************************************
//
//! Enable master mode.
//!
//! \param base is the base address of the I2S instance used.
//!
//! This function is only valid when the I2S module specified by the \b base
//! parameter is a controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_enableMasterMode(uint32_t base)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    EALLOW;

    //
    // Enable the I2S master mode.
    //
    HWREG(base + I2S_O_SCTRL) |= I2S_SCTRL_MODE;

    EDIS;
}

//*****************************************************************************
//
//! Disable master mode.
//!
//! \param base is the base address of the I2S instance used.
//!
//! This function is only valid when the I2S module specified by the \b base
//! parameter is a controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_disableMasterMode(uint32_t base)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    EALLOW;

    //
    // Disable the I2S master mode.
    //
    HWREG(base + I2S_O_SCTRL) &= ~I2S_SCTRL_MODE;

    EDIS;
}

//*****************************************************************************
//
//! Set the word length.
//!
//! \param base is the base address of the I2S instance used.
//! \param wordLength for selection of word length.
//!
//! This function is only valid when the I2S module specified by the \b base
//! parameter is a controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_setWordLength(uint32_t base, I2S_WordLength wordLength)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    EALLOW;

    //
    // Enable the I2S data delay.
    //
    HWREG(base + I2S_O_SCTRL) = (HWREG(base + I2S_O_SCTRL) & ~I2S_SCTRL_WDLEN_M) |\
    		(wordLength << I2S_SCTRL_WDLEN_S);

    EDIS;
}

//*****************************************************************************
//
//! Enable data packing.
//!
//! \param base is the base address of the I2S instance used.
//!
//! This function is only valid when the I2S module specified by the \b base
//! parameter is a controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_enableDataPacking(uint32_t base)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    EALLOW;

    //
    // Enable the I2S data packing.
    //
    HWREG(base + I2S_O_SCTRL) |= I2S_SCTRL_PACK;

    EDIS;
}

//*****************************************************************************
//
//! Disable data packing.
//!
//! \param base is the base address of the I2S instance used.
//!
//! This function is only valid when the I2S module specified by the \b base
//! parameter is a controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_disableDataPacking(uint32_t base)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    EALLOW;

    //
    // Disable the I2S data packing.
    //
    HWREG(base + I2S_O_SCTRL) &= ~I2S_SCTRL_PACK;

    EDIS;
}

//*****************************************************************************
//
//! Set the data delay bits.
//!
//! \param base is the base address of the I2S instance used.
//! \param dataDelay for selection of data delay bits.
//!
//! This function is only valid when the I2S module specified by the \b base
//! parameter is a controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_setDataDelay(uint32_t base, I2S_DataDelay dataDelay)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    EALLOW;

    //
    // Enable the I2S data delay.
    //
    HWREG(base + I2S_O_SCTRL) = (HWREG(base + I2S_O_SCTRL) & ~I2S_SCTRL_DATDLY_M) |\
    		(dataDelay << I2S_SCTRL_DATDLY_S);

    EDIS;
}

//*****************************************************************************
//
//! Enable CLK polarity inverse.
//!
//! \param base is the base address of the I2S instance used.
//!
//! This function is only valid when the I2S module specified by the \b base
//! parameter is a controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_enableCLKPolarityInverse(uint32_t base)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    EALLOW;

    //
    // Enable the I2S clock polarity invese.
    //
    HWREG(base + I2S_O_SCTRL) |= I2S_SCTRL_CLKPOL;

    EDIS;
}

//*****************************************************************************
//
//! Disable CLK polarity inverse.
//!
//! \param base is the base address of the I2S instance used.
//!
//! This function is only valid when the I2S module specified by the \b base
//! parameter is a controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_disableCLKPolarityInverse(uint32_t base)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    EALLOW;

    //
    // Disable the I2S clock polarity invese.
    //
    HWREG(base + I2S_O_SCTRL) &= ~I2S_SCTRL_CLKPOL;

    EDIS;
}

//*****************************************************************************
//
//! Enable FS polarity inverse.
//!
//! \param base is the base address of the I2S instance used.
//!
//! This function is only valid when the I2S module specified by the \b base
//! parameter is a controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_enableFSPolarityInverse(uint32_t base)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    EALLOW;

    //
    // Enable the I2S frame-synchronization polarity invese.
    //
    HWREG(base + I2S_O_SCTRL) |= I2S_SCTRL_FSPOL;

    EDIS;
}

//*****************************************************************************
//
//! Disable FS polarity inverse.
//!
//! \param base is the base address of the I2S instance used.
//!
//! This function is only valid when the I2S module specified by the \b base
//! parameter is a controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_disableFSPolarityInverse(uint32_t base)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    EALLOW;

    //
    // Disable the I2S frame-synchronization polarity invese.
    //
    HWREG(base + I2S_O_SCTRL) &= ~I2S_SCTRL_FSPOL;

    EDIS;
}

//*****************************************************************************
//
//! Enable internal loop back.
//!
//! \param base is the base address of the I2S instance used.
//!
//! This function is only valid when the I2S module specified by the \b base
//! parameter is a controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_enableLoopback(uint32_t base)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    EALLOW;

    //
    // Enable the I2S loopback.
    //
    HWREG(base + I2S_O_SCTRL) |= I2S_SCTRL_LOOPBACK;

    EDIS;
}

//*****************************************************************************
//
//! Disable internal loop back.
//!
//! \param base is the base address of the I2S instance used.
//!
//! This function is only valid when the I2S module specified by the \b base
//! parameter is a controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_disableLoopback(uint32_t base)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    EALLOW;

    //
    // Disable the I2S loopback.
    //
    HWREG(base + I2S_O_SCTRL) &= ~I2S_SCTRL_LOOPBACK;

    EDIS;
}

//*****************************************************************************
//
//! Enable mono mode.
//!
//! \param base is the base address of the I2S instance used.
//!
//! This function is only valid when the I2S module specified by the \b base
//! parameter is a controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_enableMonoMode(uint32_t base)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    EALLOW;

    //
    // Enable the I2S MONO mode.
    //
    HWREG(base + I2S_O_SCTRL) |= I2S_SCTRL_MONO;

    EDIS;
}

//*****************************************************************************
//
//! Enable stereo mode.
//!
//! \param base is the base address of the I2S instance used.
//!
//! This function is only valid when the I2S module specified by the \b base
//! parameter is a controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_disableMonoMode(uint32_t base)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    EALLOW;

    //
    // Disable the I2S MONO mode.
    //
    HWREG(base + I2S_O_SCTRL) &= ~I2S_SCTRL_MONO;

    EDIS;
}

//*****************************************************************************
//
//! Start transmission.
//!
//! \param base is the base address of the I2S instance used.
//!
//! This function is only valid when the I2S module specified by the \b base
//! parameter is a controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_enableTransmission(uint32_t base)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    EALLOW;

    //
    // Enable the I2S trasmission.
    //
    HWREG(base + I2S_O_SCTRL) |= I2S_SCTRL_ENABLE;

    EDIS;
}

//*****************************************************************************
//
//! Stop transmission.
//!
//! \param base is the base address of the I2S instance used.
//!
//! This function is only valid when the I2S module specified by the \b base
//! parameter is a controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_disableTransmission(uint32_t base)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    EALLOW;

    //
    // Disable the I2S trasmission.
    //
    HWREG(base + I2S_O_SCTRL) &= ~I2S_SCTRL_ENABLE;

    EDIS;
}

//*****************************************************************************
//
//! Enable reserved bits output.
//!
//! \param base is the base address of the I2S instance used.
//!
//! This function is only valid when the I2S module specified by the \b base
//! parameter is a controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_enableRsvBitOutput(uint32_t base)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    EALLOW;

    //
    // Enable the I2S RSV Bit Output.
    //
    HWREG(base + I2S_O_SCTRL) |= I2S_SCTRL_RESERVEBITOUT;

    EDIS;
}

//*****************************************************************************
//
//! Disable reserved bits output.
//!
//! \param base is the base address of the I2S instance used.
//!
//! This function is only valid when the I2S module specified by the \b base
//! parameter is a controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_disableRsvBitOutput(uint32_t base)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    EALLOW;

    //
    // Disable the I2S RSV Bit Output.
    //
    HWREG(base + I2S_O_SCTRL) &= ~I2S_SCTRL_RESERVEBITOUT;

    EDIS;
}

//*****************************************************************************
//
//! Enable reserved bits pad.
//!
//! \param base is the base address of the I2S instance used.
//!
//! This function is only valid when the I2S module specified by the \b base
//! parameter is a controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_enableRsvPad(uint32_t base)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    EALLOW;

    //
    // Enable the I2S RSV PAD.
    //
    HWREG(base + I2S_O_SCTRL) |= I2S_SCTRL_RSVPADEN;

    EDIS;
}

//*****************************************************************************
//
//! Disable reserved bits pad.
//!
//! \param base is the base address of the I2S instance used.
//!
//! This function is only valid when the I2S module specified by the \b base
//! parameter is a controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_disableRsvPad(uint32_t base)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    EALLOW;

    //
    // Disable the I2S RSV PAD.
    //
    HWREG(base + I2S_O_SCTRL) &= ~I2S_SCTRL_RSVPADEN;

    EDIS;
}

//*****************************************************************************
//
//! Set CLK divider.
//!
//! \param base is the base address of the I2S instance used.
//! \param divider for selection of CLK divider.
//!
//! This function is only valid when the I2S module specified by the \b base
//! parameter is a controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_setCLKDivider(uint32_t base, uint32_t divider)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));
    ASSERT(divider <= 0xFFU );

    EALLOW;

    //
    // Enable the I2S clock divider.
    //
    HWREG(base + I2S_O_SRATE) = (HWREG(base + I2S_O_SRATE) & ~I2S_SRATE_CLKDIV_M) |\
    		(divider << I2S_SRATE_CLKDIV_S);

    EDIS;
}

//*****************************************************************************
//
//! Set FS divider.
//!
//! \param base is the base address of the I2S instance used.
//! \param divider for selection of FS divider.
//!
//! This function is only valid when the I2S module specified by the \b base
//! parameter is a controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_setFSDivider(uint32_t base, I2S_FSDivider divider)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    EALLOW;

    //
    // Enable the I2S frame-synchronization divider.
    //
    HWREG(base + I2S_O_SRATE) = (HWREG(base + I2S_O_SRATE) & ~I2S_SRATE_FSDIV_M) |\
    		(divider << I2S_SRATE_FSDIV_S);

    EDIS;
}

//*****************************************************************************
//
//! Set FS pulse width.
//!
//! \param base is the base address of the I2S instance used.
//! \param width is the value for FS pulse width.
//!
//! This function is only valid when the I2S module specified by the \b base
//! parameter is a controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_setFSPulseWidth(uint32_t base, uint32_t width)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));
    ASSERT(width <= 0xFFU );

    EALLOW;

    //
    // Enable the I2S frame-synchronization pulse width.
    //
    HWREG(base + I2S_O_SRATE) = (HWREG(base + I2S_O_SRATE) & ~I2S_SRATE_FWID_M) |\
    		(width << I2S_SRATE_FWID_S);

    EDIS;
}

//*****************************************************************************
//
//! Transmits a left-word from the I2S.
//!
//! \param base is the base address of the I2S instance used.
//! \param data is the data to be transmitted from the I2S Controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_transmitLeftData0(uint32_t base, uint32_t data)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    EALLOW;

    //
    // Transmit left data 0.
    //
    HWREG(base + I2S_O_TXLT0) = data;

    EDIS;
}

//*****************************************************************************
//
//! Transmits a right-word from the I2S.
//!
//! \param base is the base address of the I2S instance used.
//! \param data is the data to be transmitted from the I2S Controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_transmitRightData0(uint32_t base, uint32_t data)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    EALLOW;

    //
    // Transmit right data 0.
    //
    HWREG(base + I2S_O_TXRT0) = data;

    EDIS;
}

//*****************************************************************************
//
//! Get the interrupt flag.
//!
//! \param base is the base address of the I2S instance used.
//!
//! \return Returns the interrupt flag as an uint32_t.
//
//*****************************************************************************
static __always_inline uint32_t
I2S_getInterruptFlag(uint32_t base)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    //
    // Get I2S interrupt flag.
    //
    return (HWREG(base + I2S_O_INTFL));
}

//*****************************************************************************
//
//! Enable mono receive interrupt.
//!
//! \param base is the base address of the I2S instance used.
//!
//! This function is only valid when the I2S module specified by the \b base
//! parameter is a controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_enableMonoReceiveInterrupt(uint32_t base)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    EALLOW;

    //
    // Enable mono receive interrupt.
    //
    HWREG(base + I2S_O_INTMASK) |= I2S_INTMASK_RCVMON;

    EDIS;
}

//*****************************************************************************
//
//! Disable mono receive interrupt.
//!
//! \param base is the base address of the I2S instance used.
//!
//! This function is only valid when the I2S module specified by the \b base
//! parameter is a controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_disableMonoReceiveInterrupt(uint32_t base)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    EALLOW;

    //
    // Disable mono receive interrupt.
    //
    HWREG(base + I2S_O_INTMASK) &= ~I2S_INTMASK_RCVMON;

    EDIS;
}

//*****************************************************************************
//
//! Enable stereo receive interrupt.
//!
//! \param base is the base address of the I2S instance used.
//!
//! This function is only valid when the I2S module specified by the \b base
//! parameter is a controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_enableStereoReceiveInterrupt(uint32_t base)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    EALLOW;

    //
    // Enable stereo receive interrupt.
    //
    HWREG(base + I2S_O_INTMASK) |= I2S_INTMASK_RCVST;

    EDIS;
}

//*****************************************************************************
//
//! Disable stereo receive interrupt.
//!
//! \param base is the base address of the I2S instance used.
//!
//! This function is only valid when the I2S module specified by the \b base
//! parameter is a controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_disableStereoReceiveInterrupt(uint32_t base)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    EALLOW;

    //
    // Disable stereo receive interrupt.
    //
    HWREG(base + I2S_O_INTMASK) &= ~I2S_INTMASK_RCVST;

    EDIS;
}

//*****************************************************************************
//
//! Enable mono transmit interrupt.
//!
//! \param base is the base address of the I2S instance used.
//!
//! This function is only valid when the I2S module specified by the \b base
//! parameter is a controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_enableMonoTransmitInterrupt(uint32_t base)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    EALLOW;

    //
    // Enable mono transmit interrupt.
    //
    HWREG(base + I2S_O_INTMASK) |= I2S_INTMASK_XMITMON;

    EDIS;
}

//*****************************************************************************
//
//! Disable mono transmit interrupt.
//!
//! \param base is the base address of the I2S instance used.
//!
//! This function is only valid when the I2S module specified by the \b base
//! parameter is a controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_disableMonoTransmitInterrupt(uint32_t base)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    EALLOW;

    //
    // Disable mono transmit interrupt.
    //
    HWREG(base + I2S_O_INTMASK) &= ~I2S_INTMASK_XMITMON;

    EDIS;
}

//*****************************************************************************
//
//! Enable stereo transmit interrupt.
//!
//! \param base is the base address of the I2S instance used.
//!
//! This function is only valid when the I2S module specified by the \b base
//! parameter is a controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_enableStereoTransmitInterrupt(uint32_t base)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    EALLOW;

    //
    // Enable stereo transmit interrupt.
    //
    HWREG(base + I2S_O_INTMASK) |= I2S_INTMASK_XMITST;

    EDIS;
}

//*****************************************************************************
//
//! Disable stereo transmit interrupt.
//!
//! \param base is the base address of the I2S instance used.
//!
//! This function is only valid when the I2S module specified by the \b base
//! parameter is a controller.
//!
//! \return None.
//
//*****************************************************************************
static __always_inline void
I2S_disableStereoTransmitInterrupt(uint32_t base)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    EALLOW;

    //
    // Disable stereo transmit interrupt.
    //
    HWREG(base + I2S_O_INTMASK) &= ~I2S_INTMASK_XMITST;

    EDIS;
}

//*****************************************************************************
//
//! Receives a left-word that has been sent to the I2S.
//!
//! \param base is the base address of the I2S instance used.
//!
//! \return Returns the left-word received from by the I2S cast as an uint32_t.
//
//*****************************************************************************
static __always_inline uint32_t
I2S_getReceiveLeftData0(uint32_t base)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    //
    // Get receive left data 0.
    //
    return (HWREG(base + I2S_O_RXLT0));
}

//*****************************************************************************
//
//! Receives a right-word that has been sent to the I2S.
//!
//! \param base is the base address of the I2S instance used.
//!
//! \return Returns the right-word received from by the I2S cast as an uint32_t.
//
//*****************************************************************************
static __always_inline uint32_t
I2S_getReceiveRightData0(uint32_t base)
{
    //
    // Check the arguments.
    //
    ASSERT(I2S_isBaseValid(base));

    //
    // Get receive right data 0.
    //
    return (HWREG(base + I2S_O_RXRT0));
}

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // I2S_H
