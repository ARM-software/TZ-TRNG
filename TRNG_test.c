/**
 * @file TRNG_Characterization.c
 *
 * This file defines the data collection function for characterization of the
 * TRNG.
 */
/****************************************************************************
* The confidential and proprietary information contained in this file may   *
* only be used by a person authorised under and to the extent permitted     *
* by a subsisting licensing agreement from Arm Limited (or its affiliates). *
*     (C) COPYRIGHT [2001-2019] Arm Limited (or its affiliates).            *
*         ALL RIGHTS RESERVED                                               *
* This entire notice must be reproduced on all copies of this file          *
* and copies of this file may only be made by a person if such person is    *
* permitted to do so under the terms of a subsisting license agreement      *
* from Arm Limited (or its affiliates).                                     *
*****************************************************************************/
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
 * 1.8 (07-Oct-2015): Renamed DX to CC
 * 1.9 (29-Oct-2019): Replace hardcoded numbers with meaningful Macros;
 *                    Add comments to improve the readbility;
 *                    TRNGMode in header changes from[31:31] to [31:30];
 *                    Change dataBuff_ptr to a callback function
 */

/* RNG module registers and value defination */
#define HW_RNG_ISR_REG_ADDR                 0x104UL
#define HW_RNG_ISR_REG_EHR_VALID            0x1
#define HW_RNG_ISR_REG_AUTOCORR_ERR         0x2
#define HW_RNG_ICR_REG_ADDR                 0x108UL
#define HW_TRNG_CONFIG_REG_ADDR             0x10CUL
#define HW_TRNG_VALID_REG_ADDR              0x110UL
#define HW_TRNG_VALID_REG_EHR_NOT_READY     0x0
#define HW_EHR_DATA_ADDR_0_REG_ADDR         0x114UL
#define HW_RND_SOURCE_ENABLE_REG_ADDR       0x12CUL
#define HW_RND_SOURCE_ENABLE_REG_SET        0x1
#define HW_RND_SOURCE_ENABLE_REG_CLR        0x0
#define HW_SAMPLE_CNT1_REG_ADDR             0x130UL
#define HW_TRNG_DEBUG_CONTROL_REG_ADDR      0x138UL
#define HW_TRNG_DEBUG_CONTROL_REG_VNC_BYPASS         0x2
#define HW_TRNG_DEBUG_CONTROL_REG_CRNGT_BYPASS       0x4
#define HW_TRNG_DEBUG_CONTROL_REG_AUTOCORR_BYPASS    0x8
#define HW_TRNG_DEBUG_CONTROL_REG_FAST      (HW_TRNG_DEBUG_CONTROL_REG_VNC_BYPASS | \
                                            HW_TRNG_DEBUG_CONTROL_REG_CRNGT_BYPASS | \
                                            HW_TRNG_DEBUG_CONTROL_REG_AUTOCORR_BYPASS)
#define HW_TRNG_DEBUG_CONTROL_REG_FE        0x0
#define HW_TRNG_DEBUG_CONTROL_REG_80090B    (HW_TRNG_DEBUG_CONTROL_REG_VNC_BYPASS | \
                                            HW_TRNG_DEBUG_CONTROL_REG_AUTOCORR_BYPASS)
#define HW_RNG_SW_RESET_REG_ADDR            0x140UL
#define HW_RNG_SW_RESET_REG_SET             0x1
#define HW_RNG_VERSION_REG_ADDR             0x1C0UL
#define HW_RNG_CLK_ENABLE_REG_ADDR          0x1C4UL
#define HW_RNG_CLK_ENABLE_REG_SET           0x1

/*
 * the output format defination
 * the output buffer consists of header + sample bits + footer
 * header: 4 words.
 *         word0/3=0xAABBCCDD
 *         word1=TRNGmode[31:30]+roscLen[25:24]+simpleLen[23:0]
 *         word2=sampleCount
 * footer: 3 words.
 *         wordn/n+2=0xDDCCBBAA
 *         wordn+1=error flags
 */
#define OUTPUT_FORMAT_HEADER_LENGTH_IN_WORDS   4
#define OUTPUT_FORMAT_FOOTER_LENGTH_IN_WORDS   3
#define OUTPUT_FORMAT_OVERHEAD_LENGTH_IN_WORDS (OUTPUT_FORMAT_HEADER_LENGTH_IN_WORDS + \
                                                OUTPUT_FORMAT_FOOTER_LENGTH_IN_WORDS)
#define OUTPUT_FORMAT_HEADER_SIG_VAL           0xAABBCCDD
#define OUTPUT_FORMAT_FOOTER_SIG_VAL           0xDDCCBBAA
#define OUTPUT_FORMAT_TRNG_MODE_SHIFT          24
#define OUTPUT_FORMAT_ROSC_LEN_SHIFT           30
#define OUTPUT_FORMAT_HEADER_LENGTH_IN_BYTES   (4*sizeof(uint32_t))
#define OUTPUT_FORMAT_FOOTER_LENGTH_IN_BYTES   (3*sizeof(uint32_t))

/* TRNG mode defination */
#define TRNG_MODE_FAST                         0
#define TRNG_MODE_FE                           1
#define TRNG_MODE_80090B                       2

/* error code defination */
#define CC_TRNG_INVALID_PARAM_TRNG_MODE        (-1)
#define CC_TRNG_INVALID_PARAM_ROSC_LEN         (-2)
#define CC_TRNG_INVALID_PARAM_SAMPLE_CNT       (-3)
#define CC_TRNG_INVALID_PARAM_BUF_SIZE         (-4)
#define CC_TRNG_INVALID_PARAM_NULL_PTR         (-5)
#define CC_TRNG_SAMPLE_LOST                    0x1

/* other size and length defination */
#define EHR_SIZE_IN_WORDS                      6
#define EHR_SIZE_IN_BYTES                      (6*sizeof(uint32_t))
#define MAX_BUFFER_LENGTH                      (1<<24)
#define MIN_BUFFER_LENGTH                      ((OUTPUT_FORMAT_OVERHEAD_LENGTH_IN_WORDS + \
                                               EHR_SIZE_IN_WORDS) * sizeof(uint32_t))
#define DATA_COLLECTION_START_INDEX            0
#define TRNG_ROSC_MAX_LENGTH                   3
#define TRNG_BUFFER_SIZE_IN_WORDS              6
#define MINIUM_SAMPLE_CNT                      1
#ifndef NULL
#define NULL  0
#endif

/* data type and operation defination */
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

/*
 * callback function type define
 * this function is called by CC_TST_TRNG() to prcess the data generated
 * or collected by CC_TST_TRNG()
 * @outputSize          the size of the data
 * @outputBuffer        buffer for the generated or collected data
*/
typedef void (*callback_TRNG)(uint32_t outputSize, uint8_t *outputBuffer);

/* report any compile errors in the next two lines to Arm */
typedef int __testCharSize[((uint8_t)~0)==255?1:-1];
typedef int __testUint32Size[(sizeof(uint32_t)==4)?1:-1];

#define CC_GEN_WriteRegister(base_addr, reg_addr, val) \
    do { ((volatile uint32_t*)(base_addr))[(reg_addr) / sizeof(uint32_t)] = (uint32_t)(val); } while(0)

#define CC_GEN_ReadRegister(base_addr, reg_addr)  ((volatile uint32_t*)(base_addr))[(reg_addr) / sizeof(uint32_t)]

/*
 * collect TRNG output for characterization
 *
 * @regBaseAddress:     base address in system memory map of TRNG registers
 * @TRNGMode:           base iteration- TRNG_MODE_FAST
 *                      1st iteration - TRNG_MODE_FAST
 *                      2nd iteration - TRNG_MODE_FE compliant with BSI AIS-31
 *                                      TRNG_MODE_80090B compliant with NIST SP 800-90B
 * @roscLength:         ring oscillator length (0 to 3)
 * @sampleCount:        ring oscillator sample counter value
 * @buffSize:           size of buffer dataBuff_ptr; must be between 52 and 2^24 bytes;
 *                      the buffsize should include the header+footer+sample_bits
 *                      buffsize >= header(16 bytes)+sample_bits/8 (bytes)+footer(12 bytes)
 *                      e.g. if 100Mbits data need to be collected, the mininal bufferSize should be
 *                      12,500,000+28=12,500,028bytes. For safe, the buffSize can be 12,501,000 bytes
 * @callbackFunc:       callback function to process the real output data
 *                      This callbackFunc will be called many times in CC_TST_TRNG
 *                      First, after 16bytes header is generated, it is called once
 *                      Then, it is be called repeatedly when the 24bytes EHR registers are full
 *                      Finally, it is called after the 12bytes footer is generated
 * @return              0 on succeeds. non-zero value on failure.
 */
int CC_TST_TRNG(    unsigned long regBaseAddress,
                    uint32_t TRNGMode,
                    uint32_t roscLength,
                    uint32_t sampleCount,
                    uint32_t buffSize,
                    callback_TRNG callbackFunc)
{
    /* LOCAL DEFINATIONS AND INITIALIZATIONS*/
    /*return value*/
    uint32_t Error = 0;

    /* loop variable */
    uint32_t i = 0;
	uint32_t j = 0;

    /* the number of full blocks needed */
    uint32_t NumOfBlocks = 0;

    /* hardware parameters */
    uint32_t EhrSizeInWords = EHR_SIZE_IN_WORDS;
    uint32_t tmpSampleCnt = 0;
    uint32_t dataArray[TRNG_BUFFER_SIZE_IN_WORDS] = {0};
    uint32_t *dataBuff_ptr = dataArray;

    /* FUNCTION LOGIC */
    /* ............... validate inputs .................................... */
    /* -------------------------------------------------------------------- */
    if (TRNGMode > TRNG_MODE_80090B)
    {
        return CC_TRNG_INVALID_PARAM_TRNG_MODE;
    }

    if (roscLength > TRNG_ROSC_MAX_LENGTH)
    {
        return CC_TRNG_INVALID_PARAM_ROSC_LEN;
    }

    if (sampleCount < MINIUM_SAMPLE_CNT)
    {
        return CC_TRNG_INVALID_PARAM_SAMPLE_CNT;
    }

    if ((buffSize < MIN_BUFFER_LENGTH) || (buffSize>= MAX_BUFFER_LENGTH))
    {
        return CC_TRNG_INVALID_PARAM_BUF_SIZE;
    }

    if (NULL == callbackFunc)
    {
        return CC_TRNG_INVALID_PARAM_NULL_PTR;
    }

    /* ........... initializing the hardware .............................. */
    /* -------------------------------------------------------------------- */
    /* reset the RNG block */
    CC_GEN_WriteRegister(regBaseAddress, HW_RNG_SW_RESET_REG_ADDR, HW_RNG_SW_RESET_REG_SET);

    /* enable RNG clock and set sample counter value untile it is set correctly*/
    do
    {
        /* enable the HW RNG clock */
        CC_GEN_WriteRegister(regBaseAddress, HW_RNG_CLK_ENABLE_REG_ADDR, HW_RNG_CLK_ENABLE_REG_SET);
        /* set sample counter value*/
        CC_GEN_WriteRegister(regBaseAddress, HW_SAMPLE_CNT1_REG_ADDR, sampleCount);
        /*read back sample counter value*/
        tmpSampleCnt = CC_GEN_ReadRegister(regBaseAddress, HW_SAMPLE_CNT1_REG_ADDR);
    }while (tmpSampleCnt != sampleCount); /* wait until the sample counter is set correctly*/

    /* set RNG rosc Length */
    CC_GEN_WriteRegister(regBaseAddress, HW_TRNG_CONFIG_REG_ADDR, roscLength);

    /* configure TRNG debug control register based on different mode. */
    if (TRNGMode == TRNG_MODE_FAST)
    {
        /* fast TRNG: bypass VNC, CRNGT and auto correlate, activate none. */
        CC_GEN_WriteRegister(regBaseAddress, HW_TRNG_DEBUG_CONTROL_REG_ADDR , HW_TRNG_DEBUG_CONTROL_REG_FAST);
    }
    else if (TRNGMode == TRNG_MODE_FE)
    {
        /* FE TRNG: bypass none, activate all */
        CC_GEN_WriteRegister(regBaseAddress, HW_TRNG_DEBUG_CONTROL_REG_ADDR , HW_TRNG_DEBUG_CONTROL_REG_FE);
    }
    else if (TRNGMode == TRNG_MODE_80090B)
    {
        /* 800-90B TRNG: bypass VNC and auto correlate, activate CRNGT */
        CC_GEN_WriteRegister(regBaseAddress, HW_TRNG_DEBUG_CONTROL_REG_ADDR , HW_TRNG_DEBUG_CONTROL_REG_80090B);
    }

    /* enable the RND source */
    CC_GEN_WriteRegister(regBaseAddress, HW_RND_SOURCE_ENABLE_REG_ADDR, HW_RND_SOURCE_ENABLE_REG_SET);

    /* ........... executing the RND operation ............................ */
    /* -------------------------------------------------------------------- */
    /* write header into buffer */
    *(dataBuff_ptr++) = OUTPUT_FORMAT_HEADER_SIG_VAL;
    *(dataBuff_ptr++) = (TRNGMode << OUTPUT_FORMAT_ROSC_LEN_SHIFT) |
                        (roscLength << OUTPUT_FORMAT_TRNG_MODE_SHIFT) |
                        buffSize;
    *(dataBuff_ptr++) = sampleCount;
    *(dataBuff_ptr++) = OUTPUT_FORMAT_HEADER_SIG_VAL;
    callbackFunc(OUTPUT_FORMAT_HEADER_LENGTH_IN_BYTES, (uint8_t *)dataArray);
    dataBuff_ptr = dataArray;

    /* calculate the number of full blocks needed */
    NumOfBlocks = (buffSize/sizeof(uint32_t) - OUTPUT_FORMAT_OVERHEAD_LENGTH_IN_WORDS) / EhrSizeInWords;

    /* fill the Output buffer with up to full blocks */
    /* BEGIN TIMING: start time measurement at this point */
    for (i = 0; i < NumOfBlocks; i++)
    {
        uint32_t valid_at_start, valid;
        /*
         * TRNG data collecting must be continuous.
         * verification methodology:
         * 1) initial state detection is vaild_at_start == HW_TRNG_VALID_REG_EHR_NOT_READY
         * 2) proceed only after HW_RNG_ISR_REG_EHR_VALID bit changed from 0 to 1
         *    (that means EHR data is ready for reading)
         */
        valid_at_start = CC_GEN_ReadRegister(regBaseAddress, HW_TRNG_VALID_REG_ADDR);
        valid = valid_at_start;
        /*
         * wait for EHR valid. ISR_REG indicates whether EHR valid or any error detected.
         * bit[0]-EHR_VALID, bit[1]-AUTOCORR_ERR, bit[2]-CRNGT_ERR, bit[3]-VN_ERR
         */
        while ((valid & (HW_RNG_ISR_REG_EHR_VALID|HW_RNG_ISR_REG_AUTOCORR_ERR)) == 0x0)
        {
            valid = CC_GEN_ReadRegister(regBaseAddress, HW_RNG_ISR_REG_ADDR);
        }

        /*
         * after EHR data is ready, first check any error detected
         * different bit in variable Error indicates different error
         * Error bit[0] samples were lost during collection.
         * Error bit[1] autocorrelation error. corresponding to ISR_REG[1].
         *              this error will cause TRNG cease to function until next reset
         * Error bit[2] CRNGT error. corresponding to ISR_REG[2].
         *              This error occurs when 2 consecutive blocks of 16 collected bits are equal.
         * Error bit[3] Von Neumann error. corresponding to ISR_REG[3].
         *              this error occurs if 32 consecutive collected bits are identical.
         */
        /* if it is not the first interation, valid_at_start must be not ready. otherwise it means samples were lost */
        if ((valid_at_start != HW_TRNG_VALID_REG_EHR_NOT_READY) && (i != DATA_COLLECTION_START_INDEX))
        {
            Error = CC_TRNG_SAMPLE_LOST;
        }

        /*pass bit[31:1] to Error. mask out bit[0] which is used for CC_TRNG_SAMPLE_LOST*/
        if ((valid & ~CC_TRNG_SAMPLE_LOST) != 0)
        {
            Error |= (valid & ~CC_TRNG_SAMPLE_LOST);
        }

        if (Error & HW_RNG_ISR_REG_AUTOCORR_ERR)
        {
            break; /* autocorrelation error is irrecoverable */
        }

        /* clean up interrupt status */
        CC_GEN_WriteRegister(regBaseAddress, HW_RNG_ICR_REG_ADDR, ~0UL);

        /*
         * load the current random data from EHR registers to the output buffer.
         * NOTE: TRNG hardware will auto start to re-fill bits to EHR_DATA_REG0-5
         *       after the host read out all 6 registers
         */
        for (j=0; j<EhrSizeInWords; j++)
        {
            *(dataBuff_ptr++) = CC_GEN_ReadRegister(regBaseAddress, HW_EHR_DATA_ADDR_0_REG_ADDR+(j*sizeof(uint32_t)));
        }
        callbackFunc(EHR_SIZE_IN_BYTES, (uint8_t *)dataArray);
        dataBuff_ptr = dataArray;
    }
    /* END TIMING: end time measurement at this point */

    /* write footer into buffer */
    *(dataBuff_ptr++) = OUTPUT_FORMAT_FOOTER_SIG_VAL;
    /* only record sample lost error in output buffer */
    *(dataBuff_ptr++) = Error & CC_TRNG_SAMPLE_LOST;
    *(dataBuff_ptr++) = OUTPUT_FORMAT_FOOTER_SIG_VAL;
    callbackFunc(OUTPUT_FORMAT_FOOTER_LENGTH_IN_BYTES, (uint8_t *)dataArray);

    /* disable the RND source */
    CC_GEN_WriteRegister(regBaseAddress, HW_RND_SOURCE_ENABLE_REG_ADDR, HW_RND_SOURCE_ENABLE_REG_CLR);

    /* .............. end of function ..................................... */
    /* -------------------------------------------------------------------- */
    return Error;
}
