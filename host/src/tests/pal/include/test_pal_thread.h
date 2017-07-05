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

#ifndef TEST_PAL_THREAD_H_
#define TEST_PAL_THREAD_H_

#include <stdint.h>

typedef void *ThreadHandle;

/****************************************************************************/
/*   							External API  								*/
/****************************************************************************/
/*
 * @brief This function creates a thread.
 *
 * @param[in] stack size in bytes, thread function, function input arguments
 * thread name and name length.
 *
 * @param[out]
 *
 * @return threadHandle address for success, NULL for failure.
 */
ThreadHandle Test_PalThreadCreate(size_t stackSize, void *(*threadFunc) (void *), void *args, char *threadName, uint8_t nameLen);

/****************************************************************************/
/*
 * @brief This function waits for a thread to terminate.
 * If that thread has already terminated it returns immediately.
 *
 * @param[in] threadHandle and threadRet (the exit status of
 * the target thread).
 *
 * @param[out]
 *
 * @return rc - 0 for success, 1 for failure.
 */
uint32_t Test_PalThreadJoin(ThreadHandle threadHandle, void *threadRet);

/****************************************************************************/
/*
 * @brief This function destroys thread.
 *
 * @param[in] threadHandle.
 * @param[out]
 *
 * @return rc - 0 for success, 1 for failure.
 */
uint32_t Test_PalThreadDestroy(ThreadHandle threadHandle);

#endif /* TEST_PAL_THREAD_H_ */
