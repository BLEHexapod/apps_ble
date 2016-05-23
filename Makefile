PROJECT_NAME := $(shell basename $(CURDIR))

export OUTPUT_FILENAME
MAKEFILE_NAME := $(MAKEFILE_LIST)
MAKEFILE_DIR := $(dir $(MAKEFILE_NAME))
SDK_ROOT := $(shell echo $$SDK_ROOT)
SDK_LIBS := $(shell echo $$SDK_LIBS)
PROJ_HOME := $(SDK_ROOT)/apps/$(PROJECT_NAME)

TEMPLATE_PATH = $(SDK_ROOT)/components/toolchain/gcc
include $(TEMPLATE_PATH)/Makefile.posix

MK := mkdir
RM := rm -rf

#echo suspend
ifeq ("$(VERBOSE)","1")
NO_ECHO :=
else
NO_ECHO := @
endif

# Toolchain commands
CC              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-gcc'
AS              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-as'
AR              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-ar' -r
LD              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-ld'
NM              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-nm'
OBJDUMP         := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objdump'
OBJCOPY         := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objcopy'
SIZE            := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-size'

#function for removing duplicates in a list
remduplicates = $(strip $(if $1,$(firstword $1) $(call remduplicates,$(filter-out $(firstword $1),$1))))

#Project specific files
INC_PATHS  	+= -I$(abspath $(PROJ_HOME)/config)
INC_PATHS  	+= -I$(abspath $(PROJ_HOME)/include)
INC_PATHS	+= -I$(abspath $(SDK_LIBS)/freertos/include)
C_SOURCE_FILES += $(abspath $(PROJ_HOME)/main.c)
C_SOURCE_FILES += $(abspath $(wildcard $(PROJ_HOME)/src/*))
C_SOURCE_FILES += $(abspath $(wildcard $(SDK_LIBS)/freertos/src/*))

OBJECT_DIRECTORY = _build
LISTING_DIRECTORY = $(OBJECT_DIRECTORY)
OUTPUT_BINARY_DIRECTORY = $(OBJECT_DIRECTORY)

# Sorting removes duplicates
BUILD_DIRECTORIES := $(sort $(OBJECT_DIRECTORY) $(OUTPUT_BINARY_DIRECTORY) $(LISTING_DIRECTORY) )

ifeq ("$(DEBUG)","1")
    CFLAGS = -DDEBUG -g3 -O0
else
    CFLAGS = -DNDEBUG -Os -Werror
endif


clean:
	$(RM) $(BUILD_DIRECTORIES)

cleanobj:
	$(RM) $(BUILD_DIRECTORIES)/*.o

ifeq ($(DEVICE), NRF51)
include Makefile.nrf51
else
ifeq ($(DEVICE), NRF52)
include Makefile.nrf52
endif
endif