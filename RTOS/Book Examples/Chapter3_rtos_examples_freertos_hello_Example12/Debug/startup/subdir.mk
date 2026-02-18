################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../startup/startup_mkl25z4.c 

C_DEPS += \
./startup/startup_mkl25z4.d 

OBJS += \
./startup/startup_mkl25z4.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.c startup/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MKL25Z128VLK4_cm0plus -DCPU_MKL25Z128VLK4 -DDEBUG -DFSL_RTOS_FREE_RTOS -DFRDM_KL25Z -DFREEDOM -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DSDK_OS_FREE_RTOS -I"C:\Users\jorge\Documents\MCUXpressoIDE_11.8.0_1165\workspace\Chapter3_rtos_examples_freertos_hello_Example12\drivers" -I"C:\Users\jorge\Documents\MCUXpressoIDE_11.8.0_1165\workspace\Chapter3_rtos_examples_freertos_hello_Example12\freertos" -I"C:\Users\jorge\Documents\MCUXpressoIDE_11.8.0_1165\workspace\Chapter3_rtos_examples_freertos_hello_Example12\CMSIS" -I"C:\Users\jorge\Documents\MCUXpressoIDE_11.8.0_1165\workspace\Chapter3_rtos_examples_freertos_hello_Example12\utilities" -I"C:\Users\jorge\Documents\MCUXpressoIDE_11.8.0_1165\workspace\Chapter3_rtos_examples_freertos_hello_Example12\source" -I"C:\Users\jorge\Documents\MCUXpressoIDE_11.8.0_1165\workspace\Chapter3_rtos_examples_freertos_hello_Example12" -I"C:\Users\jorge\Documents\MCUXpressoIDE_11.8.0_1165\workspace\Chapter3_rtos_examples_freertos_hello_Example12\startup" -I"C:\Users\jorge\Documents\MCUXpressoIDE_11.8.0_1165\workspace\Chapter3_rtos_examples_freertos_hello_Example12\board" -O0 -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-startup

clean-startup:
	-$(RM) ./startup/startup_mkl25z4.d ./startup/startup_mkl25z4.o

.PHONY: clean-startup

