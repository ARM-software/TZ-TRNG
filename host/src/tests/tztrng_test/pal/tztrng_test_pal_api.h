/****************************************************************************
 * The confidential and proprietary information contained in this file may    *
 * only be used by a person authorised under and to the extent permitted      *
 * by a subsisting licensing agreement from ARM Limited or its affiliates.    *
 * 	(C) COPYRIGHT [2001-2016] ARM Limited or its affiliates.	     *
 *	    ALL RIGHTS RESERVED						     *
 * This entire notice must be reproduced on all copies of this file           *
 * and copies of this file may only be made by a person if such person is     *
 * permitted to do so under the terms of a subsisting license agreement       *
 * from ARM Limited or its affiliates.					     *
 *****************************************************************************/

#ifndef _TZTRNG_TEST_HAL__API_H_
#define _TZTRNG_TEST_HAL__API_H_

/**
 * @brief Undo actions that were done while getting the hardware register address
 * Finalize the use of any resource.
 * Should be called before exiting the application.
 *
 * @param regBase		The address to which the hardware base register was mapped to.
 */
void tztrngTest_pal_unmapCcRegs(unsigned long regBase);

/**
 * This function should be called before calling tztrng API.
 * This function maps the HW base register to application memory.
 * In Linux will be mapped to virtual mempry.
 * In freeRTOS mapping in not needed since we have access to memory's physical address.
 *
 * @param regBase		HW_BASE_REG. The offset of the HW base register.
 *
 * @return 				An address to which the HW_BASE_REG was mapped to.
 */
unsigned long tztrngTest_pal_mapCcRegs(unsigned long regBase);

/**
 * Dump the data from large_buf to OS specific output.
 *
 * @param large_buf
 * @param outputLen
 *
 * @return
 */
int tztrngTest_pal_dumpData(unsigned char *large_buf, size_t outputLen);

#endif //_TZTRNG_TEST_HAL__API_H_
