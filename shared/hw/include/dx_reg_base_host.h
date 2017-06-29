/****************************************************************************
* The confidential and proprietary information contained in this file may    *
* only be used by a person authorised under and to the extent permitted      *
* by a subsisting licensing agreement from ARM Limited or its affiliates.    *
* 	(C) COPYRIGHT [2001-2017] ARM Limited or its affiliates.	     *
*	    ALL RIGHTS RESERVED						     *
* This entire notice must be reproduced on all copies of this file           *
* and copies of this file may only be made by a person if such person is     *
* permitted to do so under the terms of a subsisting license agreement       *
* from ARM Limited or its affiliates.					     *
*****************************************************************************/

#ifndef __DX_REG_BASE_HOST_H__
#define __DX_REG_BASE_HOST_H__

/*! 
@file
@brief This file contains general HW related definitions. 
*/

/*! Identify platform: Xilinx Zynq7000 ZC706 */
#define DX_PLAT_ZYNQ7000 1
#define DX_PLAT_ZYNQ7000_ZC706 1

/*! SEP core clock frequency in MHz */
#define DX_SEP_FREQ_MHZ 50

/*! Base address for Sansa Silicon-Secure registers. It must be modified to match the mapping of Sansa Silicon-Secure on the customer's *  
*   platform.*/
#define DX_BASE_RNG 0x40000000

/*! Base address for FPGA environment registers */
#define DX_BASE_ENV_REGS 0x80008000

#endif /*__DX_REG_BASE_HOST_H__*/
