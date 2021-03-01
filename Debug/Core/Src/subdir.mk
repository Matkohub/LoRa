################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Init.c \
../Core/Src/LoRa.c \
../Core/Src/main.c \
../Core/Src/stm32g0xx_hal_msp.c \
../Core/Src/stm32g0xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/system_stm32g0xx.c 

OBJS += \
./Core/Src/Init.o \
./Core/Src/LoRa.o \
./Core/Src/main.o \
./Core/Src/stm32g0xx_hal_msp.o \
./Core/Src/stm32g0xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/system_stm32g0xx.o 

C_DEPS += \
./Core/Src/Init.d \
./Core/Src/LoRa.d \
./Core/Src/main.d \
./Core/Src/stm32g0xx_hal_msp.d \
./Core/Src/stm32g0xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/system_stm32g0xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -mfloat-abi=soft -DUSE_HAL_DRIVER -DSTM32G030xx -I"C:/Users/MZrnic/Documents/STM Projects/LoRaV2/Core/Inc" -I"C:/Users/MZrnic/Documents/STM Projects/LoRaV2/Drivers/STM32G0xx_HAL_Driver/Inc" -I"C:/Users/MZrnic/Documents/STM Projects/LoRaV2/Drivers/STM32G0xx_HAL_Driver/Inc/Legacy" -I"C:/Users/MZrnic/Documents/STM Projects/LoRaV2/Drivers/CMSIS/Device/ST/STM32G0xx/Include" -I"C:/Users/MZrnic/Documents/STM Projects/LoRaV2/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


