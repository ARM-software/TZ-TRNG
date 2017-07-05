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

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "test_pal_thread.h"
#include "test_pal_mem.h"
#include "test_pal_log_plat.h"
#include "FreeRTOS.h"
#include "task.h"
#include "projdefs.h"

typedef struct
{
        TaskHandle_t taskHandles;
        TaskHandle_t parentTaskHandles; // Handle of the task who called Test_PalThreadCreate
        void *(*taskFunc) (void *);
        void *args;
} ThreadStr;

#define TASK_PRIORITY    	  tskIDLE_PRIORITY

/****************************************************************************/
/*   							Internal API  								*/
/****************************************************************************/
void Test_PalThreadFunc(void *pvParameter)
{
	/* Calls task function */
	ThreadStr *threadStr = pvParameter;
	threadStr->taskFunc(threadStr->args);

	/* Sends a notification for the parent task */
	if(threadStr->parentTaskHandles != NULL)
		xTaskNotifyGive(threadStr->parentTaskHandles);

    /* suicide */
    vTaskDelete( NULL );
}

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
ThreadHandle Test_PalThreadCreate(size_t stackSize, void *(*threadFunc) (void *), void *args, char * threadName, uint8_t nameLen)
{
	char buff[8];
	size_t stackSizeInWords;
	ThreadStr *threadStr;

	threadStr = Test_PalMalloc(sizeof(ThreadStr));
	if(threadStr == NULL)
	{
		TEST_PRINTF_ERROR("threadStr allocation failed\n");
		return NULL;
	}

	memset(buff, 0, sizeof(buff));
	memcpy(buff, threadName, ((nameLen > 8) ? 8:nameLen));

	stackSizeInWords = (stackSize + 1) >> 1;

	threadStr->taskFunc = threadFunc;
	threadStr->args = args;

	if(xTaskCreate(Test_PalThreadFunc, buff, stackSizeInWords, threadStr, tskIDLE_PRIORITY, &threadStr->taskHandles) == pdFAIL)
	{
		TEST_PRINTF_ERROR("thread creation failed\n");
		return NULL;
	}

	/* New task was created. Save current task handle for future notifications */
	threadStr->parentTaskHandles = xTaskGetCurrentTaskHandle();
	return threadStr;
}

/****************************************************************************/
/*
 * @brief This function waits for a thread to terminate.
 * If that thread has already terminated it returns immediately.
 *
 * @param[in] ThreadHandle (not in use) and threadRet (the exit status of
 * the target thread).
 *
 * @param[out]
 *
 * @return rc - 0 for success, 1 for failure.
 */
uint32_t Test_PalThreadJoin(ThreadHandle threadHandle, void *threadRet)
{
	ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
	return 0;
}

/****************************************************************************/
/*
 * @brief This function frees threadHandle.
 *
 * @param[in] threadStr.
 * @param[out]
 *
 * @return 0.
 */
uint32_t Test_PalThreadDestroy(ThreadHandle threadHandle)
{
	Test_PalFree(threadHandle);
	return 0;
}
