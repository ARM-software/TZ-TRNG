HOST_PROJ_ROOT ?= $(shell pwd)/../../..

ifeq ($(CROSS_COMPILE),$(filter $(CROSS_COMPILE),arm-ds5 arm-none-eabi-))
TEE_OS = freertos
endif

include $(HOST_PROJ_ROOT)/Makefile.defs

ifeq ($(TEE_OS),$(filter $(TEE_OS),linux cc_linux))
TARGET_EXES = tztrng_test
DEPLIBS = cc_tztrng
else
TARGET_LIBS = tztrng_test
TEST_INCLUDES += $(CURDIR)/tztrng_test.h

INCDIRS_EXTRA += $(KERNEL_DIR)/OS/FreeRTOS/Source/include
INCDIRS_EXTRA += $(KERNEL_DIR)/board/MPS2+
INCDIRS_EXTRA += $(KERNEL_DIR)/lib/main
INCDIRS_EXTRA += $(KERNEL_DIR)/OS/FreeRTOS/Source/portable/RVDS/ARM_CM3
INCDIRS_EXTRA += $(KERNEL_DIR)/arch/cmsis/CMSIS/Include
TEE_OS = freertos
endif

SOURCES_tztrng_test = tztrng_test.c
SOURCES_tztrng_test += tztrng_test_pal.c 

INCDIRS_EXTRA += $(HOST_PROJ_ROOT)/src/tztrng_lib/include
INCDIRS_EXTRA += $(CURDIR)/pal

#since TEE_OS for linux is cc_linux the include dir is actually $(CURDIR)/pal/linux
ifneq (,$(findstring linux,$(TEE_OS)))
INCDIRS_EXTRA += $(CURDIR)/pal/linux
else
ifeq ($(CROSS_COMPILE),arm-none-eabi-)
INCDIRS_EXTRA += $(CURDIR)/pal/freertos
else
INCDIRS_EXTRA += $(CURDIR)/pal/$(TEE_OS)
endif 
endif
INCDIRS_EXTRA += $(SHARED_DIR)/hw/include


include $(HOST_PROJ_ROOT)/Makefile.rules
# We should flatten the components source trees to avoid long search paths...
VPATH += $(CURDIR)/pal
ifneq (,$(findstring linux,$(TEE_OS)))
VPATH += $(CURDIR)/pal/linux
else
ifeq ($(CROSS_COMPILE),arm-none-eabi-)
VPATH += $(CURDIR)/pal/freertos
else
VPATH += $(CURDIR)/pal/$(TEE_OS)
endif
endif

