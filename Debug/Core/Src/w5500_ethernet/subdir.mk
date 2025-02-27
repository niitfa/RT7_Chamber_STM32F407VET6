################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/w5500_ethernet/socket.c \
../Core/Src/w5500_ethernet/w5500.c \
../Core/Src/w5500_ethernet/wizchip_conf.c \
../Core/Src/w5500_ethernet/wiznet_api.c 

OBJS += \
./Core/Src/w5500_ethernet/socket.o \
./Core/Src/w5500_ethernet/w5500.o \
./Core/Src/w5500_ethernet/wizchip_conf.o \
./Core/Src/w5500_ethernet/wiznet_api.o 

C_DEPS += \
./Core/Src/w5500_ethernet/socket.d \
./Core/Src/w5500_ethernet/w5500.d \
./Core/Src/w5500_ethernet/wizchip_conf.d \
./Core/Src/w5500_ethernet/wiznet_api.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/w5500_ethernet/%.o Core/Src/w5500_ethernet/%.su Core/Src/w5500_ethernet/%.cyclo: ../Core/Src/w5500_ethernet/%.c Core/Src/w5500_ethernet/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Core/Src/general_task -I../Core/Src/keyboard -I../Core/Src/tcp_server -I../Core/Src/w5500_ethernet -I../Core/Src/display_contents/screens -I../Core/Src/display_contents/widgets -I../Core/Src/display_contents/ILI9341 -I../Core/Src/adc -I../Core/Src/dac -I../Core/Src/high_voltage_system -I../Core/Src/other -I../Core/Src/ssd1306 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-w5500_ethernet

clean-Core-2f-Src-2f-w5500_ethernet:
	-$(RM) ./Core/Src/w5500_ethernet/socket.cyclo ./Core/Src/w5500_ethernet/socket.d ./Core/Src/w5500_ethernet/socket.o ./Core/Src/w5500_ethernet/socket.su ./Core/Src/w5500_ethernet/w5500.cyclo ./Core/Src/w5500_ethernet/w5500.d ./Core/Src/w5500_ethernet/w5500.o ./Core/Src/w5500_ethernet/w5500.su ./Core/Src/w5500_ethernet/wizchip_conf.cyclo ./Core/Src/w5500_ethernet/wizchip_conf.d ./Core/Src/w5500_ethernet/wizchip_conf.o ./Core/Src/w5500_ethernet/wizchip_conf.su ./Core/Src/w5500_ethernet/wiznet_api.cyclo ./Core/Src/w5500_ethernet/wiznet_api.d ./Core/Src/w5500_ethernet/wiznet_api.o ./Core/Src/w5500_ethernet/wiznet_api.su

.PHONY: clean-Core-2f-Src-2f-w5500_ethernet

