//###########################################################################
//
// FILE:   clb.c
//
// TITLE:  H28x CLB driver.
//
//###########################################################################


#include "clb.h"

//*****************************************************************************
//
// CLB_configCounterLoadMatch
//
//*****************************************************************************
void CLB_configCounterLoadMatch(uint32_t base, CLB_Counters counterID,
                               uint32_t load, uint32_t match1, uint32_t match2)
{
    ASSERT(CLB_isBaseValid(base));

    EALLOW;
    switch(counterID)
    {
        case CLB_CTR0:
            CLB_writeInterface(base, CLB_ADDR_COUNTER_0_LOAD, load);
            CLB_writeInterface(base, CLB_ADDR_COUNTER_0_MATCH1, match1);
            CLB_writeInterface(base, CLB_ADDR_COUNTER_0_MATCH2, match2);
            break;

        case CLB_CTR1:
            CLB_writeInterface(base, CLB_ADDR_COUNTER_1_LOAD, load);
            CLB_writeInterface(base, CLB_ADDR_COUNTER_1_MATCH1, match1);
            CLB_writeInterface(base, CLB_ADDR_COUNTER_1_MATCH2, match2);
            break;

        case CLB_CTR2:
            CLB_writeInterface(base, CLB_ADDR_COUNTER_2_LOAD, load);
            CLB_writeInterface(base, CLB_ADDR_COUNTER_2_MATCH1, match1);
            CLB_writeInterface(base, CLB_ADDR_COUNTER_2_MATCH2, match2);
            break;

        default:
            //
            // Invalid counterID value
            //
            break;
    }
    EDIS;
}

//*****************************************************************************
//
// CLB_clearFIFOs
//
//*****************************************************************************
void CLB_clearFIFOs(uint32_t base)
{
    uint32_t i;

    ASSERT(CLB_isBaseValid(base));

    for(i = 0U; i < CLB_FIFO_SIZE; i++)
    {
        HWREG(base + CLB_DATAEXCH + CLB_O_PULL(i)) = 0U;
    }

    HWREG(base + CLB_LOGICCTL + CLB_O_BUF_PTR) = 0U;
}

//*****************************************************************************
//
// CLB_writeFIFOs
//
//*****************************************************************************
void CLB_writeFIFOs(uint32_t base , const uint32_t pullData[])
{
    ASSERT(CLB_isBaseValid(base));

    //
    // Clear the FIFO and pointer
    //
    CLB_clearFIFOs(base);

    //
    // Write data into the FIFO.
    //
    HWREG(base + CLB_DATAEXCH + CLB_O_PULL(0U)) = pullData[0U];
    HWREG(base + CLB_DATAEXCH + CLB_O_PULL(1U)) = pullData[1U];
    HWREG(base + CLB_DATAEXCH + CLB_O_PULL(2U)) = pullData[2U];
    HWREG(base + CLB_DATAEXCH + CLB_O_PULL(3U)) = pullData[3U];
}

//*****************************************************************************
//
// CLB_readFIFOs
//
//*****************************************************************************
void CLB_readFIFOs(uint32_t base , uint32_t pushData[])
{
    ASSERT(CLB_isBaseValid(base));

    //
    // Read data from the FIFO.
    //
    pushData[0U] = HWREG(base + CLB_DATAEXCH + CLB_O_PUSH(0U)) ;
    pushData[1U] = HWREG(base + CLB_DATAEXCH + CLB_O_PUSH(1U)) ;
    pushData[2U] = HWREG(base + CLB_DATAEXCH + CLB_O_PUSH(2U)) ;
    pushData[3U] = HWREG(base + CLB_DATAEXCH + CLB_O_PUSH(3U)) ;
}


