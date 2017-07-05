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

#include <stdio.h>
#include "FreeRTOS.h"
#include "FreeRTOS_CLI.h"
#include "test_pal_cli.h"

/****************************************************************************/
/*   							External API  								*/
/****************************************************************************/
/*
 * @brief This function registers a new CLI command.
 *
 * @param[in] commandToRegister - command structure.
 *
 * @param[out] paramStringLength
 *
 * @return - 0 for success and 1 for failure.
 */
uint32_t Test_PalCLIRegisterCommand(Test_PalCliCommand *commandToRegister)
{
	if(FreeRTOS_CLIRegisterCommand((CLI_Command_Definition_t *)commandToRegister) == pdFAIL)
		return 1;

	return 0;
}

/****************************************************************************/
/*
 * @brief This function obtains a parameter string by its index and stores the parameter string length.
 *
 * @param[in] commandString - The command string itself.
 * 			  wantedParamIndx - parameter index.
 *
 * @param[out] paramStringLength
 *
 * @return parameter string.
 */
const char *Test_PalCLIGetParameter(const char *commandString, uint32_t wantedParamIndx, uint32_t *paramStringLength)
{
	return FreeRTOS_CLIGetParameter(commandString, wantedParamIndx, (BaseType_t *)paramStringLength);
}
