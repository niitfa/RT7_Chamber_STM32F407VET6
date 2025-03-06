################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/general_task/general_task.c 

OBJS += \
./Core/Src/general_task/general_task.o 

C_DEPS += \
./Core/Src/general_task/general_task.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/general_task/%.o Core/Src/general_task/%.su Core/Src/general_task/%.cyclo: ../Core/Src/general_task/%.c Core/Src/general_task/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Core/Src/general_task -I../Core/Src/keyboard -I../Core/Src/tcp_server -I../Core/Src/w5500_ethernet -I../Core/Src/display_contents/screens -I../Core/Src/display_contents/widgets -I../Core/Src/display_contents/ILI9341 -I../Core/Src/adc -I../Core/Src/dac -I../Core/Src/high_voltage_system -I../Core/Src/other -I../Core/Src/ssd1306 -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I../Core/Src/sd -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-general_task

clean-Core-2f-Src-2f-general_task:
	-$(RM) ./Core/Src/general_task/general_task.cyclo ./Core/Src/general_task/general_task.d ./Core/Src/general_task/general_task.o ./Core/Src/general_task/general_task.su

.PHONY: clean-Core-2f-Src-2f-general_task

