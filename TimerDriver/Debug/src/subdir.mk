################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/DR_EINT.c \
../src/DR_PLL.c \
../src/DR_SysTick.c \
../src/DR_Teclados.c \
../src/DR_Timer.c \
../src/DR_UART0.c \
../src/DR_gpio.c \
../src/DR_pinsel.c \
../src/PR_Teclado.c \
../src/PR_UART.c \
../src/cr_startup_lpc175x_6x.c \
../src/crp.c \
../src/main.c 

OBJS += \
./src/DR_EINT.o \
./src/DR_PLL.o \
./src/DR_SysTick.o \
./src/DR_Teclados.o \
./src/DR_Timer.o \
./src/DR_UART0.o \
./src/DR_gpio.o \
./src/DR_pinsel.o \
./src/PR_Teclado.o \
./src/PR_UART.o \
./src/cr_startup_lpc175x_6x.o \
./src/crp.o \
./src/main.o 

C_DEPS += \
./src/DR_EINT.d \
./src/DR_PLL.d \
./src/DR_SysTick.d \
./src/DR_Teclados.d \
./src/DR_Timer.d \
./src/DR_UART0.d \
./src/DR_gpio.d \
./src/DR_pinsel.d \
./src/PR_Teclado.d \
./src/PR_UART.d \
./src/cr_startup_lpc175x_6x.d \
./src/crp.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__LPC17XX__ -D__REDLIB__ -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


