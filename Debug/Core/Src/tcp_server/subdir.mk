################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/tcp_server/rx_message.c \
../Core/Src/tcp_server/tcp_input_stream.c \
../Core/Src/tcp_server/tcp_output_stream.c \
../Core/Src/tcp_server/tx_message.c 

OBJS += \
./Core/Src/tcp_server/rx_message.o \
./Core/Src/tcp_server/tcp_input_stream.o \
./Core/Src/tcp_server/tcp_output_stream.o \
./Core/Src/tcp_server/tx_message.o 

C_DEPS += \
./Core/Src/tcp_server/rx_message.d \
./Core/Src/tcp_server/tcp_input_stream.d \
./Core/Src/tcp_server/tcp_output_stream.d \
./Core/Src/tcp_server/tx_message.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/tcp_server/%.o Core/Src/tcp_server/%.su Core/Src/tcp_server/%.cyclo: ../Core/Src/tcp_server/%.c Core/Src/tcp_server/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Core/Src/general_task -I../Core/Src/keyboard -I../Core/Src/tcp_server -I../Core/Src/w5500_ethernet -I../Core/Src/display_contents/screens -I../Core/Src/display_contents/widgets -I../Core/Src/display_contents/ILI9341 -I../Core/Src/adc -I../Core/Src/dac -I../Core/Src/high_voltage_system -I../Core/Src/other -I../Core/Src/ssd1306 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-tcp_server

clean-Core-2f-Src-2f-tcp_server:
	-$(RM) ./Core/Src/tcp_server/rx_message.cyclo ./Core/Src/tcp_server/rx_message.d ./Core/Src/tcp_server/rx_message.o ./Core/Src/tcp_server/rx_message.su ./Core/Src/tcp_server/tcp_input_stream.cyclo ./Core/Src/tcp_server/tcp_input_stream.d ./Core/Src/tcp_server/tcp_input_stream.o ./Core/Src/tcp_server/tcp_input_stream.su ./Core/Src/tcp_server/tcp_output_stream.cyclo ./Core/Src/tcp_server/tcp_output_stream.d ./Core/Src/tcp_server/tcp_output_stream.o ./Core/Src/tcp_server/tcp_output_stream.su ./Core/Src/tcp_server/tx_message.cyclo ./Core/Src/tcp_server/tx_message.d ./Core/Src/tcp_server/tx_message.o ./Core/Src/tcp_server/tx_message.su

.PHONY: clean-Core-2f-Src-2f-tcp_server

