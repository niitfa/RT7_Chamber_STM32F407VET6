################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/display_contents/ILI9341/ILI9341_GFX.c \
../Core/Src/display_contents/ILI9341/ILI9341_STM32_Driver.c \
../Core/Src/display_contents/ILI9341/fonts.c 

OBJS += \
./Core/Src/display_contents/ILI9341/ILI9341_GFX.o \
./Core/Src/display_contents/ILI9341/ILI9341_STM32_Driver.o \
./Core/Src/display_contents/ILI9341/fonts.o 

C_DEPS += \
./Core/Src/display_contents/ILI9341/ILI9341_GFX.d \
./Core/Src/display_contents/ILI9341/ILI9341_STM32_Driver.d \
./Core/Src/display_contents/ILI9341/fonts.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/display_contents/ILI9341/%.o Core/Src/display_contents/ILI9341/%.su Core/Src/display_contents/ILI9341/%.cyclo: ../Core/Src/display_contents/ILI9341/%.c Core/Src/display_contents/ILI9341/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Core/Src/general_task -I../Core/Src/keyboard -I../Core/Src/tcp_server -I../Core/Src/w5500_ethernet -I../Core/Src/display_contents/screens -I../Core/Src/display_contents/widgets -I../Core/Src/display_contents/ILI9341 -I../Core/Src/adc -I../Core/Src/dac -I../Core/Src/high_voltage_system -I../Core/Src/other -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-display_contents-2f-ILI9341

clean-Core-2f-Src-2f-display_contents-2f-ILI9341:
	-$(RM) ./Core/Src/display_contents/ILI9341/ILI9341_GFX.cyclo ./Core/Src/display_contents/ILI9341/ILI9341_GFX.d ./Core/Src/display_contents/ILI9341/ILI9341_GFX.o ./Core/Src/display_contents/ILI9341/ILI9341_GFX.su ./Core/Src/display_contents/ILI9341/ILI9341_STM32_Driver.cyclo ./Core/Src/display_contents/ILI9341/ILI9341_STM32_Driver.d ./Core/Src/display_contents/ILI9341/ILI9341_STM32_Driver.o ./Core/Src/display_contents/ILI9341/ILI9341_STM32_Driver.su ./Core/Src/display_contents/ILI9341/fonts.cyclo ./Core/Src/display_contents/ILI9341/fonts.d ./Core/Src/display_contents/ILI9341/fonts.o ./Core/Src/display_contents/ILI9341/fonts.su

.PHONY: clean-Core-2f-Src-2f-display_contents-2f-ILI9341

