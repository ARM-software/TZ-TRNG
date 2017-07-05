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

#ifndef __TEST_PAL_LOG_PLAT_H__
#define __TEST_PAL_LOG_PLAT_H__

#include <stdint.h>
#include <stdio.h>


#define TEST_PRINTF_ERROR(format, ...)  {\
	printf("%s(): " format "\n", __func__, ##__VA_ARGS__);\
}

#define TEST_FPRINTF_ERROR  		   	TEST_PRINTF_ERROR

#define TEST_PRINTF_MESSAGE				TEST_PRINTF_ERROR

#ifdef TEST_DEBUG
#define TEST_FPRINTF				  	TEST_PRINTF_ERROR
#define TEST_PRINTF						TEST_PRINTF_ERROR

#define TEST_PRINTF1(format, ...)  {\
	printf(" " format, __func__, ##__VA_ARGS__);\
}

#define TEST_FPRINTF1(fd, format, ...)  {\
	printf(" " format, __func__, ##__VA_ARGS__);\
}

#define TEST_PRINT_WORD_BUFF(buffName, size, buff) {\
	unsigned int i = 0;\
	TEST_PRINTF("printing %s, word size %d", buffName, (unsigned int)size);\
	for (i=0; i< size; i++) {\
		if (!(i%4)) {\
			TEST_PRINTF1("\n\t");\
		}\
		TEST_PRINTF1("  0x%08X  ", (unsigned int)(*((unsigned int *)buff+i)));\
	}\
	TEST_PRINTF1("\n");\
}


#define TEST_PRINT_BYTE_BUFF(buffName, size, buff) {\
	unsigned int i = 0;\
	TEST_PRINTF("printing %s, byte size %d", buffName, (unsigned int)size);\
	for (i=0; i< size; i++) {\
		if (!(i%16)) {\
			TEST_PRINTF1("\n\t");\
		}\
		TEST_PRINTF1("0x%02X ", (unsigned char)(*((unsigned char *)buff+i)));\
	}\
	TEST_PRINTF1("\n");\
}

#define TEST_PRINT_BYTE_BUFFP(buffName, size, buff) {\
	unsigned int idxT = 0;\
	uint8_t *buffP = (uint8_t *)(buff);\
	TEST_PRINTF("printing %s, byte size %d", buffName, (unsigned int)size);\
	for (idxT=0; idxT< size; idxT++) {\
		if (!(idxT%16)) {\
			TEST_PRINTF1("\n\t");\
		}\
		TEST_PRINTF1("0x%02X ", buffP[idxT]);\
	}\
	TEST_PRINTF1("\n");\
}

#else
#define TEST_FPRINTF(fd, format, ...)  do{ }while(0)
#define TEST_PRINTF(format...)  do{ }while(0)
#define TEST_PRINTF1(format...)  do{ }while(0)
#define TEST_PRINT_BYTE_BUFF(str, size, buff) do{ }while(0)
#define TEST_PRINT_BYTE_BUFFP(buffName, size, buff) do{ }while(0)
#define TEST_PRINT_WORD_BUFF(str, size, buff) do{ }while(0)
#endif
#define TEST_FPRINT_LONG_NUM(fd, buffName, buff, size)  do{ }while(0)
#define TEST_FPRINT_BYTE_BUFF(fd, buffName, buff, size)  do{ }while(0)
#define TEST_FPRINT_BYTE_BUFF_MAX(fd, buffName, buff, size, maxSize)  do{ }while(0)
#endif /*__TEST_PAL_LOG_PLAT_H__*/
