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

#ifndef TEST_PAL_MEM_H_
#define TEST_PAL_MEM_H_

#include <stdint.h>
/****************************************************************************/
/*                        function declarations                             */
/****************************************************************************/
/*
 * @brief This function allocates "size" bytes.
 *
 * @param[in] size in bytes.
 *
 * @param[out]
 *
 * @return pointer to the allocated memory.
 */
void *Test_PalMalloc(size_t size);

/****************************************************************************/
/*
 * @brief This function frees allocated memory pointed by pvAddress.
 *
 * @param[in] pvAddress - pointer to the allocated memory.
 *
 * @param[out]
 *
 * @return
 */
void Test_PalFree(void *pvAddress);

/****************************************************************************/
/*
 * @brief This function changes the size of the memory block pointed by ptr.
 * If the function fails to allocate the requested block of memory,
 * a null pointer is returned and the memory block pointed by
 * argument ptr is not deallocated.
 *
 * @param[in] pointer to the allocated memory and new size.
 *
 * @param[out]
 *
 * @return a pointer to the new allocated memory.
 */
void *Test_PalRealloc(void *ptr, size_t nbytes);

/****************************************************************************/
/*
 * @brief This function allocates a DMA-contiguous buffer and returns its address.
 *
 * @param[in] size - Buffer size in bytes.
 *
 * @param[out]
 *
 * @return an address of the allocated buffer.
 */
void *Test_PalDMAContigBufferAlloc(size_t size);

/****************************************************************************/
/*
 * @brief This function frees resources previously allocated by Test_PalDMAContigBufferAlloc
 *
 * @param[in] pvAddress - address of the allocated buffer.
 *
 * @param[out]
 *
 * @return
 */
void Test_PalDMAContigBufferFree(void *pvAddress);

/****************************************************************************/
/*   						Used by TestHal					       		    */
/****************************************************************************/
/*
 * @brief This function initializes DMA buffer allocation management.
 *
 * @param[in]
 *
 * @param[out]
 *
 * @return rc - 0 for success, 1 for failure.
 */
uint32_t Test_PalMemInit(void);

/****************************************************************************/
/*
 * @brief This function ends DMA buffer allocation management.
 *
 * @param[in]
 *
 * @param[out]
 *
 * @return rc - 0 for success, 1 for failure.
 */
uint32_t Test_PalMemFin(void);

#endif /* TEST_PAL_MEM_H_ */
