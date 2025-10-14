################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/dac/dac.c \
../Core/Src/dac/dac_MCP4811_EP.c \
../Core/Src/dac/dac_emulator.c \
../Core/Src/dac/mcp4822.c 

OBJS += \
./Core/Src/dac/dac.o \
./Core/Src/dac/dac_MCP4811_EP.o \
./Core/Src/dac/dac_emulator.o \
./Core/Src/dac/mcp4822.o 

C_DEPS += \
./Core/Src/dac/dac.d \
./Core/Src/dac/dac_MCP4811_EP.d \
./Core/Src/dac/dac_emulator.d \
./Core/Src/dac/mcp4822.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/dac/%.o Core/Src/dac/%.su: ../Core/Src/dac/%.c Core/Src/dac/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Core/Src/general_task -I../Core/Src/keyboard -I../Core/Src/tcp_server -I../Core/Src/w5500_ethernet -I../Core/Src/display_contents/screens -I../Core/Src/display_contents/widgets -I../Core/Src/display_contents/ILI9341 -I../Core/Src/adc -I../Core/Src/dac -I../Core/Src/high_voltage_system -I../Core/Src/other -I../Core/Src/ssd1306 -I../Core/Src/pressure_sensor -I../Core/Src/flash_manager -I../Core/Src/uart_rx_handler -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-dac

clean-Core-2f-Src-2f-dac:
	-$(RM) ./Core/Src/dac/dac.d ./Core/Src/dac/dac.o ./Core/Src/dac/dac.su ./Core/Src/dac/dac_MCP4811_EP.d ./Core/Src/dac/dac_MCP4811_EP.o ./Core/Src/dac/dac_MCP4811_EP.su ./Core/Src/dac/dac_emulator.d ./Core/Src/dac/dac_emulator.o ./Core/Src/dac/dac_emulator.su ./Core/Src/dac/mcp4822.d ./Core/Src/dac/mcp4822.o ./Core/Src/dac/mcp4822.su

.PHONY: clean-Core-2f-Src-2f-dac

