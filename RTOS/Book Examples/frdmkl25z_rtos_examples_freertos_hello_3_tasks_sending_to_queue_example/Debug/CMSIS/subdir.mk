################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/system_MKL25Z4.c 

C_DEPS += \
./CMSIS/system_MKL25Z4.d 

OBJS += \
./CMSIS/system_MKL25Z4.o 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/%.o: ../CMSIS/%.c CMSIS/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MKL25Z128VLK4_cm0plus -DCPU_MKL25Z128VLK4 -DDEBUG -DFSL_RTOS_FREE_RTOS -DFRDM_KL25Z -DFREEDOM -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DSDK_OS_FREE_RTOS -I"C:\Users\jorge\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmkl25z_rtos_examples_freertos_hello_3_tasks_sending_to_queue_example\drivers" -I"C:\Users\jorge\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmkl25z_rtos_examples_freertos_hello_3_tasks_sending_to_queue_example\freertos" -I"C:\Users\jorge\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmkl25z_rtos_examples_freertos_hello_3_tasks_sending_to_queue_example\CMSIS" -I"C:\Users\jorge\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmkl25z_rtos_examples_freertos_hello_3_tasks_sending_to_queue_example\utilities" -I"C:\Users\jorge\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmkl25z_rtos_examples_freertos_hello_3_tasks_sending_to_queue_example\source" -I"C:\Users\jorge\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmkl25z_rtos_examples_freertos_hello_3_tasks_sending_to_queue_example" -I"C:\Users\jorge\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmkl25z_rtos_examples_freertos_hello_3_tasks_sending_to_queue_example\startup" -I"C:\Users\jorge\Documents\MCUXpressoIDE_11.8.0_1165\workspace\frdmkl25z_rtos_examples_freertos_hello_3_tasks_sending_to_queue_example\board" -O0 -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-CMSIS

clean-CMSIS:
	-$(RM) ./CMSIS/system_MKL25Z4.d ./CMSIS/system_MKL25Z4.o

.PHONY: clean-CMSIS

