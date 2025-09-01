################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/flash_manager/flash_data.c 

OBJS += \
./Core/Src/flash_manager/flash_data.o 

C_DEPS += \
./Core/Src/flash_manager/flash_data.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/flash_manager/%.o Core/Src/flash_manager/%.su Core/Src/flash_manager/%.cyclo: ../Core/Src/flash_manager/%.c Core/Src/flash_manager/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Core/Src/general_task -I../Core/Src/keyboard -I../Core/Src/tcp_server -I../Core/Src/w5500_ethernet -I../Core/Src/display_contents/screens -I../Core/Src/display_contents/widgets -I../Core/Src/display_contents/ILI9341 -I../Core/Src/adc -I../Core/Src/dac -I../Core/Src/high_voltage_system -I../Core/Src/other -I../Core/Src/ssd1306 -I../Core/Src/pressure_sensor -I../Core/Src/flash_manager -I../Core/Src/uart_rx_handler -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-flash_manager

clean-Core-2f-Src-2f-flash_manager:
	-$(RM) ./Core/Src/flash_manager/flash_data.cyclo ./Core/Src/flash_manager/flash_data.d ./Core/Src/flash_manager/flash_data.o ./Core/Src/flash_manager/flash_data.su

.PHONY: clean-Core-2f-Src-2f-flash_manager

