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
#ifndef _CC_CONFIG_TRNG90B_H
#define _CC_CONFIG_TRNG90B_H

/*
This file should be updated according to the characterization process.
*/

/*
Requirements:
- Required entropy = 384 bits

Default values for Zynq FPGA:
- entropy per bit = 0.5
*/

/* amount of bytes for the required entropy bits = ROUND_UP(ROUND_UP(((required entropy bits)/(entropy per bit)), 1024), (EHR width in bytes)) / 8
   (multiple of the window size 1024 bits and multiple of the EHR width 192 bits) */
#define CC_CONFIG_TRNG90B_AMOUNT_OF_BYTES                      144  /* ROUND_UP(ROUND_UP((384/0.5), 1024), 192) / 8 = 144 */

/*** NIST SP 800-90B (2nd Draft) 4.4.1 ***/
/* C = ROUND_UP(1+(-log(W)/H)), W = 2^(-40), H=(entropy per bit) */
#define CC_CONFIG_TRNG90B_REPETITION_COUNTER_CUTOFF            81  /* ROUND_UP(1+(40/0.5)) = 81 */

/*** NIST SP 800-90B (2nd Draft) 4.4.2 ***/
/* C =CRITBINOM(W, power(2,(-H)),1-a), W = 1024, a = 2^(-40), H=(entropy per bit) */
#define CC_CONFIG_TRNG90B_ADAPTIVE_PROPORTION_CUTOFF           823      /* =CRITBINOM(1024, power(2,(-0.5)),1-2^(-40)) */

/*** For Startup Tests ***/
/* amount of bytes for the startup test = 528 (at least 4096 bits (NIST SP 800-90B (2nd Draft) 4.3.12) = 22 EHRs = 4224 bits) */
#define CC_CONFIG_TRNG90B_AMOUNT_OF_BYTES_STARTUP              528

/* sample count for each ring oscillator */
/* for unallowed rosc, sample count = 0 */
#define CC_CONFIG_SAMPLE_CNT_ROSC_1		200
#define CC_CONFIG_SAMPLE_CNT_ROSC_2		200
#define CC_CONFIG_SAMPLE_CNT_ROSC_3		300
#define CC_CONFIG_SAMPLE_CNT_ROSC_4		100

#endif

