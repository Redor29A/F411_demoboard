######################################
# target
######################################
TARGET = F411_demo

DEBUG = 1
OPT = -Og

BUILD_DIR = build

######################################
# sources
######################################

# C++ sources
CPP_SOURCES = \
Src/main.cpp

# C sources (оставляем HAL и прочее)
C_SOURCES =  \
Src/system_stm32f4xx.c \
Src/sysmem.c \
Src/syscalls.c

ASM_SOURCES = startup_stm32f411xe.s

#######################################
# toolchain
#######################################
PREFIX = arm-none-eabi-

ifdef GCC_PATH
CC  = $(GCC_PATH)/$(PREFIX)gcc
CXX = $(GCC_PATH)/$(PREFIX)g++
AS  = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP  = $(GCC_PATH)/$(PREFIX)objcopy
SZ  = $(GCC_PATH)/$(PREFIX)size
else
CC  = $(PREFIX)gcc
CXX = $(PREFIX)g++
AS  = $(PREFIX)gcc -x assembler-with-cpp
CP  = $(PREFIX)objcopy
SZ  = $(PREFIX)size
endif

HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#######################################
# MCU flags
#######################################
CPU = -mcpu=cortex-m4
FPU = -mfpu=fpv4-sp-d16
FLOAT-ABI = -mfloat-abi=hard
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

C_DEFS = -DSTM32F411xE

C_INCLUDES = \
-IInc \
-IDrivers/STM32F4xx_HAL_Driver/Inc \
-IDrivers/STM32F4xx_HAL_Driver/Inc/Legacy \
-IDrivers/CMSIS/Device/ST/STM32F4xx/Include \
-IDrivers/CMSIS/Include

#######################################
# flags
#######################################
COMMON_FLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall \
-fdata-sections -ffunction-sections

CFLAGS   = $(COMMON_FLAGS)
CXXFLAGS = $(COMMON_FLAGS) -fno-exceptions -fno-rtti

ifeq ($(DEBUG),1)
CFLAGS   += -g -gdwarf-2
CXXFLAGS += -g -gdwarf-2
endif

#######################################
# linker
#######################################
LDSCRIPT = STM32F411XX_FLASH.ld

LIBS = -lc -lm -lnosys
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBS) \
-Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

#######################################
# objects
#######################################

OBJECTS = \
$(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o))) \
$(addprefix $(BUILD_DIR)/,$(notdir $(CPP_SOURCES:.cpp=.o))) \
$(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))

vpath %.c   $(sort $(dir $(C_SOURCES)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))
vpath %.s   $(sort $(dir $(ASM_SOURCES)))

#######################################
# build rules
#######################################

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.s | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf
	$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf
	$(BIN) $< $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)