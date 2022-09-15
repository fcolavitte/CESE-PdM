################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/API/src/API_debounce.c \
../Drivers/API/src/API_delay.c 

OBJS += \
./Drivers/API/src/API_debounce.o \
./Drivers/API/src/API_delay.o 

C_DEPS += \
./Drivers/API/src/API_debounce.d \
./Drivers/API/src/API_delay.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/API/src/%.o Drivers/API/src/%.su: ../Drivers/API/src/%.c Drivers/API/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F429ZITx -DSTM32F4 -DNUCLEO_F429ZI -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Inc -I"C:/Users/Facu/Desktop/UBA Esp en Sist Emb/Materias CESE/PdM/CESE-PdM/Practica_4_punto2-PdM/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Facu/Desktop/UBA Esp en Sist Emb/Materias CESE/PdM/CESE-PdM/Practica_4_punto2-PdM/Drivers/CMSIS/Include" -I"C:/Users/Facu/Desktop/UBA Esp en Sist Emb/Materias CESE/PdM/CESE-PdM/Practica_4_punto2-PdM/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Facu/Desktop/UBA Esp en Sist Emb/Materias CESE/PdM/CESE-PdM/Practica_4_punto2-PdM/Drivers/BSP/STM32F4xx_Nucleo_144" -I"C:/Users/Facu/Desktop/UBA Esp en Sist Emb/Materias CESE/PdM/CESE-PdM/Practica_4_punto2-PdM/Drivers/Core/Inc" -I"C:/Users/Facu/Desktop/UBA Esp en Sist Emb/Materias CESE/PdM/CESE-PdM/Practica_4_punto2-PdM/Drivers/API/inc" -I"C:/Users/Facu/Desktop/UBA Esp en Sist Emb/Materias CESE/PdM/CESE-PdM/Practica_4_punto2-PdM/Drivers/API/src" -I"C:/Users/Facu/Desktop/UBA Esp en Sist Emb/Materias CESE/PdM/CESE-PdM/Practica_4_punto2-PdM/Drivers/API" -I"C:/Users/Facu/Desktop/UBA Esp en Sist Emb/Materias CESE/PdM/CESE-PdM/Practica_4_punto2-PdM/Drivers" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-API-2f-src

clean-Drivers-2f-API-2f-src:
	-$(RM) ./Drivers/API/src/API_debounce.d ./Drivers/API/src/API_debounce.o ./Drivers/API/src/API_debounce.su ./Drivers/API/src/API_delay.d ./Drivers/API/src/API_delay.o ./Drivers/API/src/API_delay.su

.PHONY: clean-Drivers-2f-API-2f-src

