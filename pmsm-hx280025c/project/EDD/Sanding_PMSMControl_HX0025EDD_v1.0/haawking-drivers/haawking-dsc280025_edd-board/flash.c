//###########################################################################
//
// FILE:   flash.c
//
// TITLE:  H28x Flash driver.
//
//###########################################################################


#include "flash.h"


//*****************************************************************************
//
// Flash_initModule
//
//*****************************************************************************
void CODE_SECTION("ramfuncs")
Flash_initModule(uint32_t ctrlBase, uint32_t eccBase, uint16_t waitstates)
{
    //
    // Check the arguments.
    //
    ASSERT(Flash_isCtrlBaseValid(ctrlBase));
    ASSERT(Flash_isECCBaseValid(eccBase));
    ASSERT(waitstates <= 0xFU);

    //
    // Set the bank fallback power mode to active.
    //
    Flash_setBankPowerMode(ctrlBase, FLASH_BANK, FLASH_BANK_PWR_ACTIVE);

    //
    // Disable cache and prefetch mechanism before changing wait states
    //
    Flash_disableCache(ctrlBase);
    Flash_disablePrefetch(ctrlBase);

    //
    // Set waitstates according to frequency.
    //
    Flash_setWaitstates(ctrlBase, waitstates);

    //
    // Enable cache and prefetch mechanism to improve performance of code
    // executed from flash.
    //
    Flash_enableCache(ctrlBase);
    Flash_enablePrefetch(ctrlBase);

    //
    // At reset, ECC is enabled.  If it is disabled by application software and
    // if application again wants to enable ECC.
    //
    Flash_enableECC(eccBase);

    //
    // Force a pipeline flush to ensure that the write to the last register
    // configured occurs before returning.
    //
    FLASH_DELAY_CONFIG;
}

//*****************************************************************************
//
// Flash_powerDown
//
//*****************************************************************************
void CODE_SECTION("ramfuncs")
Flash_powerDown(uint32_t ctrlBase)
{
    //
    // Check the arguments.
    //
    ASSERT(Flash_isCtrlBaseValid(ctrlBase));

    //
    // Power down the flash bank.
    //
    Flash_setBankPowerMode(ctrlBase, FLASH_BANK, FLASH_BANK_PWR_SLEEP);
}

//*****************************************************************************
//
// Flash_wakeFromLPM
//
//*****************************************************************************
void CODE_SECTION("ramfuncs")
Flash_wakeFromLPM(uint32_t ctrlBase)
{
    //
    // Check the arguments.
    //
    ASSERT(Flash_isCtrlBaseValid(ctrlBase));

    //
    // Set the bank fallback power modes to active.
    //
    Flash_setBankPowerMode(ctrlBase, FLASH_BANK, FLASH_BANK_PWR_ACTIVE);
}

