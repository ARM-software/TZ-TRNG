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

#ifndef TEST_PAL_MAP_ADDRS_H_
#define TEST_PAL_MAP_ADDRS_H_


/****************************************************************************/
/*                        function declarations                             */
/****************************************************************************/
/*                        	Used by TestHal                                 */
/****************************************************************************/
/*
 * @brief This function maps a physical address to a virtual address.
 * @param[in] physical address, preferred static address for mapping, filename, filename size in bytes
 * and prot and flags bit mask.
 *
 * @param[out]
 *
 * @return a valid virtual address or null in case of failure.
 */
void *Test_PalMapAddr(void *physAddr, void *startingAddr, const char *filename, size_t size, uint8_t protAndFlagsBitMask);

/****************************************************************************/
/*
 * @brief This function unmaps a virtual address.
 * @param[in] virtual address and size in bytes.
 *
 * @param[out]
 *
 * @return
 */
void Test_PalUnmapAddr(void *virtAddr, size_t size);

#endif /* TEST_PAL_MAP_ADDRS_H_ */
