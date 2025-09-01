################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/uart_rx_handler/uart_rx_handler.c 

OBJS += \
./Core/Src/uart_rx_handler/uart_rx_handler.o 

C_DEPS += \
./Core/Src/uart_rx_handler/uart_rx_handler.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/uart_rx_handler/%.o Core/Src/uart_rx_handler/%.su Core/Src/uart_rx_handler/%.cyclo: ../Core/Src/uart_rx_handler/%.c Core/Src/uart_rx_handler/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Core/Src/general_task -I../Core/Src/keyboard -I../Core/Src/tcp_server -I../Core/Src/w5500_ethernet -I../Core/Src/display_contents/screens -I../Core/Src/display_contents/widgets -I../Core/Src/display_contents/ILI9341 -I../Core/Src/adc -I../Core/Src/dac -I../Core/Src/high_voltage_system -I../Core/Src/other -I../Core/Src/ssd1306 -I../Core/Src/pressure_sensor -I../Core/Src/flash_manager -I../Core/Src/uart_rx_handler -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-uart_rx_handler

clean-Core-2f-Src-2f-uart_rx_handler:
	-$(RM) ./Core/Src/uart_rx_handler/uart_rx_handler.cyclo ./Core/Src/uart_rx_handler/uart_rx_handler.d ./Core/Src/uart_rx_handler/uart_rx_handler.o ./Core/Src/uart_rx_handler/uart_rx_handler.su

.PHONY: clean-Core-2f-Src-2f-uart_rx_handler

