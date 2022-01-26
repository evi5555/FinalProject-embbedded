################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/catch.c \
../Src/led.c \
../Src/main.c \
../Src/uart.c \
../Src/wifi.c 

OBJS += \
./Src/catch.o \
./Src/led.o \
./Src/main.o \
./Src/uart.o \
./Src/wifi.o 

C_DEPS += \
./Src/catch.d \
./Src/led.d \
./Src/main.d \
./Src/uart.d \
./Src/wifi.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F3 -DDEBUG -DSTM32F303RETx -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

