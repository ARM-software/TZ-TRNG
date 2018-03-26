/******************************************************************************
 * Copyright (c) 2017-2017, ARM, All Rights Reserved                           *
 * SPDX-License-Identifier: Apache-2.0                                         *
 *                                                                             *
 * Licensed under the Apache License, Version 2.0 (the "License");             *
 * you may not use this file except in compliance with the License.            *
 *                                                                             *
 * You may obtain a copy of the License at                                     *
 * http://www.apache.org/licenses/LICENSE-2.0                                  *
 *                                                                             *
 * Unless required by applicable law or agreed to in writing, software         *
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT   *
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.            *
 *                                                                             *
 * See the License for the specific language governing permissions and         *
 * limitations under the License.                                              *
 ******************************************************************************/
/**
 * @file TRNG_test.c
 *
 * This file defines the data collection function for characterization of the
 * Arm TrustZone TRNG.
 */
/*
 * Revision history:
 * 1.0 (16-Mar-2010): Initial revision
 * 1.1 (11-Jul-2010): Added extra metadata to output format
 * 1.2 (18-Jul-2010): Fixed reset sequence
 *                    Changed sampling rate metadata to 32-bit
 * 1.3 (09-Jun-2011): Added 8 bit processor support
 * 1.4 (07-Feb-2013): Only 192bit EHR support
 * 1.5 (03-Mar-2013): Replace isSlowMode with TRNGMode
 * 1.6 (06-Mar-2013): Support both DxTRNG and CryptoCell4.4
 * 1.7 (14-May-2015): Support for 800-90B
 * 1.8 (07-Oct-2015):  renamed DX to CC
 */

#define HW_RNG_ISR_REG_ADDR                 0x104UL
#define HW_RNG_ICR_REG_ADDR                 0x108UL
#define HW_TRNG_CONFIG_REG_ADDR             0x10CUL
#define HW_TRNG_VALID_REG_ADDR              0x110UL
#define HW_EHR_DATA_ADDR_0_REG_ADDR         0x114UL
#define HW_RND_SOURCE_ENABLE_REG_ADDR       0x12CUL
#define HW_SAMPLE_CNT1_REG_ADDR             0x130UL
#define HW_TRNG_DEBUG_CONTROL_REG_ADDR      0x138UL
#define HW_RNG_SW_RESET_REG_ADDR            0x140UL
#define HW_RNG_VERSION_REG_ADDR             0x1C0UL
#define HW_RNG_CLK_ENABLE_REG_ADDR          0x1C4UL

typedef unsigned int DxUint32_t;
/* report any compile errors in the next two lines to Arm */
typedef int __testCharSize[((unsigned char)~0)==255?1:-1];
typedef int __testDxUint32Size[(sizeof(DxUint32_t)==4)?1:-1];

#define CC_GEN_WriteRegister(base_addr, reg_addr, val) \
    do { ((volatile DxUint32_t*)(base_addr))[(reg_addr) / sizeof(DxUint32_t)] = (DxUint32_t)(val); } while(0)

#define CC_GEN_ReadRegister(base_addr, reg_addr)  ((volatile DxUint32_t*)(base_addr))[(reg_addr) / sizeof(DxUint32_t)]


/**
 * Collect TRNG output for characterization
 *
 * @regBaseAddress:     Base address in system memory map of TRNG registers
 * @TRNGMode:           0 - FTRNG; 1 – MM TRNG driver; 2 – NIST TRNG driver
 * @roscLength:         Ring oscillator length (0 to 3)
 * @sampleCount:        Ring oscillator sampling rate
 * @buffSize:           Size of buffer passed in dataBuff_ptr; must be between 52 and 2^24 bytes
 * @dataBuff_ptr:       Buffer for results
 *
 * The function's output is 0 if the collection succeeds, or a (non-zero) error code on failure.
 */
int CC_TST_TRNG(    unsigned long regBaseAddress,
                    unsigned int TRNGMode,
                    unsigned int roscLength,
                    unsigned int sampleCount,
                    unsigned int buffSize,
                    DxUint32_t *dataBuff_ptr)
{
    /* LOCAL DECLARATIONS */

    unsigned int Error;

    /* loop variable */
    unsigned int i,j;

    /* The number of full blocks needed */
    unsigned int NumOfBlocks;

    /* Hardware parameters */
    unsigned int EhrSizeInWords;
    unsigned int tmpSamplCnt = 0;

    /* FUNCTION LOGIC */

    /* ............... local initializations .............................. */
    /* -------------------------------------------------------------------- */

    EhrSizeInWords = 6;

    /* ............... validate inputs .................................... */
    /* -------------------------------------------------------------------- */

    if (buffSize < (7 + EhrSizeInWords) * sizeof(DxUint32_t)) return -1;
    if (buffSize >= (1<<24)) return -1;

    /* ........... initializing the hardware .............................. */
    /* -------------------------------------------------------------------- */

	/* enable the HW RND clock   */
	CC_GEN_WriteRegister( regBaseAddress, HW_RNG_CLK_ENABLE_REG_ADDR, 0x1);


    /* reset the RNG block */
    CC_GEN_WriteRegister( regBaseAddress, HW_RNG_SW_RESET_REG_ADDR, 0x1 );

	do{
		/* enable the HW RND clock   */
		CC_GEN_WriteRegister( regBaseAddress, HW_RNG_CLK_ENABLE_REG_ADDR, 0x1);

		/* set sampling ratio (rng_clocks) between consequtive bits */
		CC_GEN_WriteRegister( regBaseAddress, HW_SAMPLE_CNT1_REG_ADDR, sampleCount);

		/* read the sampling ratio  */
		tmpSamplCnt = CC_GEN_ReadRegister( regBaseAddress, HW_SAMPLE_CNT1_REG_ADDR );
	}while (tmpSamplCnt != sampleCount);

    /* enable the RNG clock  */

    /* configure RNG */
    CC_GEN_WriteRegister( regBaseAddress, HW_TRNG_CONFIG_REG_ADDR, roscLength & 0x03 );

     if ( TRNGMode == 0 )
    {
        /* For fast TRNG: VNC_BYPASS + TRNG_CRNGT_BYPASS + AUTO_CORRELATE_BYPASS */
        CC_GEN_WriteRegister( regBaseAddress, HW_TRNG_DEBUG_CONTROL_REG_ADDR , 0x0000000E);
    }
	 else if(TRNGMode == 2)//800-90B
    {
        /* For 800-90B TRNG: VNC_BYPASS + TRNG_CRNGT_BYPASS + AUTO_CORRELATE_BYPASS */
        CC_GEN_WriteRegister( regBaseAddress, HW_TRNG_DEBUG_CONTROL_REG_ADDR , 0x0000000A);
    }else if(TRNGMode == 1)
    {
        /* For TRNGS: enable all hardware test blocks */
        CC_GEN_WriteRegister( regBaseAddress, HW_TRNG_DEBUG_CONTROL_REG_ADDR , 0x00000000);
    }

    /* enable the RNG source  */
    CC_GEN_WriteRegister( regBaseAddress, HW_RND_SOURCE_ENABLE_REG_ADDR, 0x1 );

    /* ........... executing the RND operation ............................ */
    /* -------------------------------------------------------------------- */

    /* Write header into buffer */
    *(dataBuff_ptr++) = 0xAABBCCDD;
    *(dataBuff_ptr++) = (TRNGMode << 31) |
                        (roscLength << 24) |
                        buffSize;
    *(dataBuff_ptr++) = sampleCount;
    *(dataBuff_ptr++) = 0xAABBCCDD;

    /* The number of full blocks needed */
    NumOfBlocks = (buffSize/sizeof(DxUint32_t) - 7) / EhrSizeInWords;

    Error = 0;

    /* fill the Output buffer with up to full blocks */
    /* BEGIN TIMING: start time measurement at this point */
    for( i = 0 ; i < NumOfBlocks ; i++ )
    {
        DxUint32_t valid_at_start, valid;
        /*wait for RND ready*/
        valid_at_start = CC_GEN_ReadRegister( regBaseAddress, HW_TRNG_VALID_REG_ADDR );
        valid = valid_at_start;
        while( (valid & 0x3) == 0x0 )
            valid = CC_GEN_ReadRegister( regBaseAddress, HW_RNG_ISR_REG_ADDR );
        if ( (valid_at_start != 0) && (i != 0) ) Error = 1;
        if ( (valid & ~1) != 0 ) Error |= (valid & ~1);
		if ( Error & 0x2 ) break; /* autocorrelation error is irrecoverable */
			CC_GEN_WriteRegister( regBaseAddress, HW_RNG_ICR_REG_ADDR, ~0 );

        for (j=0;j<EhrSizeInWords;j++)
        {
            /* load the current random data to the output buffer */
            *(dataBuff_ptr++) = CC_GEN_ReadRegister( regBaseAddress, HW_EHR_DATA_ADDR_0_REG_ADDR+(j*sizeof(DxUint32_t)) );
        }

        // another read to reset the bits-counter
        valid = CC_GEN_ReadRegister( regBaseAddress, HW_RNG_ISR_REG_ADDR );

    }
    /* END TIMING: end time measurement at this point */

    /* Write trailer into buffer */
    *(dataBuff_ptr++) = 0xDDCCBBAA;
	/* check that no bit was skipped during collection */
    *(dataBuff_ptr++) = Error & 0x1;

    *(dataBuff_ptr++) = 0xDDCCBBAA;

    /* disable the RND source  */
    CC_GEN_WriteRegister( regBaseAddress, HW_RND_SOURCE_ENABLE_REG_ADDR, 0x0 );

    /* close the Hardware clock */
    //CC_GEN_WriteRegister( regBaseAddress, HW_RNG_CLK_ENABLE_REG_ADDR , 0x0 );

    /* .............. end of function ..................................... */
    /* -------------------------------------------------------------------- */

    return Error;
}

