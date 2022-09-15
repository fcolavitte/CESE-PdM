################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Startup/startup_stm32f429xx.s 

OBJS += \
./Startup/startup_stm32f429xx.o 

S_DEPS += \
./Startup/startup_stm32f429xx.d 


# Each subdirectory must supply rules for building sources it contributes
Startup/%.o: ../Startup/%.s Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"C:/Users/Facu/Desktop/UBA Esp en Sist Emb/Materias CESE/PdM/CESE-PdM/Practica_4_punto2-PdM/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Facu/Desktop/UBA Esp en Sist Emb/Materias CESE/PdM/CESE-PdM/Practica_4_punto2-PdM/Drivers/CMSIS/Include" -I"C:/Users/Facu/Desktop/UBA Esp en Sist Emb/Materias CESE/PdM/CESE-PdM/Practica_4_punto2-PdM/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Facu/Desktop/UBA Esp en Sist Emb/Materias CESE/PdM/CESE-PdM/Practica_4_punto2-PdM/Drivers/BSP/STM32F4xx_Nucleo_144" -I"C:/Users/Facu/Desktop/UBA Esp en Sist Emb/Materias CESE/PdM/CESE-PdM/Practica_4_punto2-PdM/Inc" -I"C:/Users/Facu/Desktop/UBA Esp en Sist Emb/Materias CESE/PdM/CESE-PdM/Practica_4_punto2-PdM/Drivers/Core/Inc" -I"C:/Users/Facu/Desktop/UBA Esp en Sist Emb/Materias CESE/PdM/CESE-PdM/Practica_4_punto2-PdM/Drivers/API/inc" -I"C:/Users/Facu/Desktop/UBA Esp en Sist Emb/Materias CESE/PdM/CESE-PdM/Practica_4_punto2-PdM/Drivers/API/src" -I"C:/Users/Facu/Desktop/UBA Esp en Sist Emb/Materias CESE/PdM/CESE-PdM/Practica_4_punto2-PdM/Drivers/API" -I"C:/Users/Facu/Desktop/UBA Esp en Sist Emb/Materias CESE/PdM/CESE-PdM/Practica_4_punto2-PdM/Drivers" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Startup

clean-Startup:
	-$(RM) ./Startup/startup_stm32f429xx.d ./Startup/startup_stm32f429xx.o

.PHONY: clean-Startup

