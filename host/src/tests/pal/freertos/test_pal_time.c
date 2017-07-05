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

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/****************************************************************************/
/*   							External API  								*/
/****************************************************************************/
/*
 * @brief This function suspends execution of the calling thread
 * for microsecond intervals.
 *
 * @param[in] time to suspend in microseconds.
 *
 * @param[out]
 *
 * @return
 */
void Test_PalDelay(const uint32_t msec)
{
	vTaskDelay(msec / portTICK_PERIOD_MS);
}
