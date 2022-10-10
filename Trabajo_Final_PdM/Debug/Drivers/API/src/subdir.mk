################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/API/src/API_debounce.c \
../Drivers/API/src/API_delay.c \
../Drivers/API/src/API_dht22.c \
../Drivers/API/src/API_num_strings.c \
../Drivers/API/src/API_pwm.c \
../Drivers/API/src/API_uart.c 

OBJS += \
./Drivers/API/src/API_debounce.o \
./Drivers/API/src/API_delay.o \
./Drivers/API/src/API_dht22.o \
./Drivers/API/src/API_num_strings.o \
./Drivers/API/src/API_pwm.o \
./Drivers/API/src/API_uart.o 

C_DEPS += \
./Drivers/API/src/API_debounce.d \
./Drivers/API/src/API_delay.d \
./Drivers/API/src/API_dht22.d \
./Drivers/API/src/API_num_strings.d \
./Drivers/API/src/API_pwm.d \
./Drivers/API/src/API_uart.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/API/src/%.o Drivers/API/src/%.su: ../Drivers/API/src/%.c Drivers/API/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F429ZITx -DSTM32F4 -DNUCLEO_F429ZI -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Inc -I"C:/Users/Facu/Desktop/UBA Esp en Sist Emb/Materias CESE/PdM/CESE-PdM/Trabajo_Final_PdM/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Facu/Desktop/UBA Esp en Sist Emb/Materias CESE/PdM/CESE-PdM/Trabajo_Final_PdM/Drivers/CMSIS/Include" -I"C:/Users/Facu/Desktop/UBA Esp en Sist Emb/Materias CESE/PdM/CESE-PdM/Trabajo_Final_PdM/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Facu/Desktop/UBA Esp en Sist Emb/Materias CESE/PdM/CESE-PdM/Trabajo_Final_PdM/Drivers/BSP/STM32F4xx_Nucleo_144" -I"C:/Users/Facu/Desktop/UBA Esp en Sist Emb/Materias CESE/PdM/CESE-PdM/Trabajo_Final_PdM/Drivers/Core/Inc" -I"C:/Users/Facu/Desktop/UBA Esp en Sist Emb/Materias CESE/PdM/CESE-PdM/Trabajo_Final_PdM/Drivers/API" -I"C:/Users/Facu/Desktop/UBA Esp en Sist Emb/Materias CESE/PdM/CESE-PdM/Trabajo_Final_PdM/Drivers" -I"C:/Users/Facu/Desktop/UBA Esp en Sist Emb/Materias CESE/PdM/CESE-PdM/Trabajo_Final_PdM/Drivers/API/inc" -I"C:/Users/Facu/Desktop/UBA Esp en Sist Emb/Materias CESE/PdM/CESE-PdM/Trabajo_Final_PdM/Drivers/API/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-API-2f-src

clean-Drivers-2f-API-2f-src:
	-$(RM) ./Drivers/API/src/API_debounce.d ./Drivers/API/src/API_debounce.o ./Drivers/API/src/API_debounce.su ./Drivers/API/src/API_delay.d ./Drivers/API/src/API_delay.o ./Drivers/API/src/API_delay.su ./Drivers/API/src/API_dht22.d ./Drivers/API/src/API_dht22.o ./Drivers/API/src/API_dht22.su ./Drivers/API/src/API_num_strings.d ./Drivers/API/src/API_num_strings.o ./Drivers/API/src/API_num_strings.su ./Drivers/API/src/API_pwm.d ./Drivers/API/src/API_pwm.o ./Drivers/API/src/API_pwm.su ./Drivers/API/src/API_uart.d ./Drivers/API/src/API_uart.o ./Drivers/API/src/API_uart.su

.PHONY: clean-Drivers-2f-API-2f-src

