################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../board/board.c \
../board/clock_config.c \
../board/pin_mux.c 

C_DEPS += \
./board/board.d \
./board/clock_config.d \
./board/pin_mux.d 

OBJS += \
./board/board.o \
./board/clock_config.o \
./board/pin_mux.o 


# Each subdirectory must supply rules for building sources it contributes
board/%.o: ../board/%.c board/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MKL25Z128VLK4_cm0plus -DCPU_MKL25Z128VLK4 -DDEBUG -DFSL_RTOS_FREE_RTOS -DFRDM_KL25Z -DFREEDOM -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DSDK_OS_FREE_RTOS -I"C:\Users\jorge\Documents\MCUXpressoIDE_11.8.0_1165\workspace\Chapter3_rtos_examples_freertos_hello_Example14\drivers" -I"C:\Users\jorge\Documents\MCUXpressoIDE_11.8.0_1165\workspace\Chapter3_rtos_examples_freertos_hello_Example14\freertos" -I"C:\Users\jorge\Documents\MCUXpressoIDE_11.8.0_1165\workspace\Chapter3_rtos_examples_freertos_hello_Example14\CMSIS" -I"C:\Users\jorge\Documents\MCUXpressoIDE_11.8.0_1165\workspace\Chapter3_rtos_examples_freertos_hello_Example14\utilities" -I"C:\Users\jorge\Documents\MCUXpressoIDE_11.8.0_1165\workspace\Chapter3_rtos_examples_freertos_hello_Example14\source" -I"C:\Users\jorge\Documents\MCUXpressoIDE_11.8.0_1165\workspace\Chapter3_rtos_examples_freertos_hello_Example14" -I"C:\Users\jorge\Documents\MCUXpressoIDE_11.8.0_1165\workspace\Chapter3_rtos_examples_freertos_hello_Example14\startup" -I"C:\Users\jorge\Documents\MCUXpressoIDE_11.8.0_1165\workspace\Chapter3_rtos_examples_freertos_hello_Example14\board" -O0 -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-board

clean-board:
	-$(RM) ./board/board.d ./board/board.o ./board/clock_config.d ./board/clock_config.o ./board/pin_mux.d ./board/pin_mux.o

.PHONY: clean-board

