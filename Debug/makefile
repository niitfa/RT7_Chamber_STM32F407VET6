################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/w5500_ethernet/subdir.mk
-include Core/Src/uart_rx_handler/subdir.mk
-include Core/Src/tcp_server/subdir.mk
-include Core/Src/ssd1306/subdir.mk
-include Core/Src/pressure_sensor/subdir.mk
-include Core/Src/keyboard/subdir.mk
-include Core/Src/high_voltage_system/subdir.mk
-include Core/Src/general_task/subdir.mk
-include Core/Src/flash_manager/subdir.mk
-include Core/Src/display_contents/widgets/subdir.mk
-include Core/Src/display_contents/screens/subdir.mk
-include Core/Src/dac/subdir.mk
-include Core/Src/adc/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := RT7_Chamber
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
RT7_Chamber.elf \

MAP_FILES += \
RT7_Chamber.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
RT7_Chamber.list \


# All Target
all: main-build

# Main-build Target
main-build: RT7_Chamber.elf secondary-outputs

# Tool invocations
RT7_Chamber.elf RT7_Chamber.map: $(OBJS) $(USER_OBJS) C:\Users\Kirill\Documents\GitHub\RT7_Chamber_STM32F407VET6\STM32F407VETX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "RT7_Chamber.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\Kirill\Documents\GitHub\RT7_Chamber_STM32F407VET6\STM32F407VETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="RT7_Chamber.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

RT7_Chamber.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "RT7_Chamber.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) RT7_Chamber.elf RT7_Chamber.list RT7_Chamber.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
