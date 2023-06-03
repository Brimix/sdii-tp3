################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/MEF.c \
../source/SD2_I2C.c \
../source/SD2_board.c \
../source/TP3_HUBER.c \
../source/key.c \
../source/mma8451.c \
../source/mtb.c \
../source/ringBuffer.c \
../source/semihost_hardfault.c \
../source/uart_ringBufer.c 

C_DEPS += \
./source/MEF.d \
./source/SD2_I2C.d \
./source/SD2_board.d \
./source/TP3_HUBER.d \
./source/key.d \
./source/mma8451.d \
./source/mtb.d \
./source/ringBuffer.d \
./source/semihost_hardfault.d \
./source/uart_ringBufer.d 

OBJS += \
./source/MEF.o \
./source/SD2_I2C.o \
./source/SD2_board.o \
./source/TP3_HUBER.o \
./source/key.o \
./source/mma8451.o \
./source/mtb.o \
./source/ringBuffer.o \
./source/semihost_hardfault.o \
./source/uart_ringBufer.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL46Z256VLL4 -DCPU_MKL46Z256VLL4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I../drivers -I../CMSIS -I../utilities -I../board -I../source -I../ -I../startup -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/MEF.d ./source/MEF.o ./source/SD2_I2C.d ./source/SD2_I2C.o ./source/SD2_board.d ./source/SD2_board.o ./source/TP3_HUBER.d ./source/TP3_HUBER.o ./source/key.d ./source/key.o ./source/mma8451.d ./source/mma8451.o ./source/mtb.d ./source/mtb.o ./source/ringBuffer.d ./source/ringBuffer.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o ./source/uart_ringBufer.d ./source/uart_ringBufer.o

.PHONY: clean-source

