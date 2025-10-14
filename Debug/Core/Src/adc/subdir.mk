################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/adc/adc.c \
../Core/Src/adc/adc_AD7791.c \
../Core/Src/adc/adc_ADS1242.c \
../Core/Src/adc/adc_ADS1246.c \
../Core/Src/adc/adc_emulator.c \
../Core/Src/adc/adc_monitor.c \
../Core/Src/adc/counter.c \
../Core/Src/adc/range_select.c 

OBJS += \
./Core/Src/adc/adc.o \
./Core/Src/adc/adc_AD7791.o \
./Core/Src/adc/adc_ADS1242.o \
./Core/Src/adc/adc_ADS1246.o \
./Core/Src/adc/adc_emulator.o \
./Core/Src/adc/adc_monitor.o \
./Core/Src/adc/counter.o \
./Core/Src/adc/range_select.o 

C_DEPS += \
./Core/Src/adc/adc.d \
./Core/Src/adc/adc_AD7791.d \
./Core/Src/adc/adc_ADS1242.d \
./Core/Src/adc/adc_ADS1246.d \
./Core/Src/adc/adc_emulator.d \
./Core/Src/adc/adc_monitor.d \
./Core/Src/adc/counter.d \
./Core/Src/adc/range_select.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/adc/%.o Core/Src/adc/%.su: ../Core/Src/adc/%.c Core/Src/adc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Core/Src/general_task -I../Core/Src/keyboard -I../Core/Src/tcp_server -I../Core/Src/w5500_ethernet -I../Core/Src/display_contents/screens -I../Core/Src/display_contents/widgets -I../Core/Src/display_contents/ILI9341 -I../Core/Src/adc -I../Core/Src/dac -I../Core/Src/high_voltage_system -I../Core/Src/other -I../Core/Src/ssd1306 -I../Core/Src/pressure_sensor -I../Core/Src/flash_manager -I../Core/Src/uart_rx_handler -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-adc

clean-Core-2f-Src-2f-adc:
	-$(RM) ./Core/Src/adc/adc.d ./Core/Src/adc/adc.o ./Core/Src/adc/adc.su ./Core/Src/adc/adc_AD7791.d ./Core/Src/adc/adc_AD7791.o ./Core/Src/adc/adc_AD7791.su ./Core/Src/adc/adc_ADS1242.d ./Core/Src/adc/adc_ADS1242.o ./Core/Src/adc/adc_ADS1242.su ./Core/Src/adc/adc_ADS1246.d ./Core/Src/adc/adc_ADS1246.o ./Core/Src/adc/adc_ADS1246.su ./Core/Src/adc/adc_emulator.d ./Core/Src/adc/adc_emulator.o ./Core/Src/adc/adc_emulator.su ./Core/Src/adc/adc_monitor.d ./Core/Src/adc/adc_monitor.o ./Core/Src/adc/adc_monitor.su ./Core/Src/adc/counter.d ./Core/Src/adc/counter.o ./Core/Src/adc/counter.su ./Core/Src/adc/range_select.d ./Core/Src/adc/range_select.o ./Core/Src/adc/range_select.su

.PHONY: clean-Core-2f-Src-2f-adc

