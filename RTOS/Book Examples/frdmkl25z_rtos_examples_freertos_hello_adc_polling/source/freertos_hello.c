/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "pin_mux.h"
#include "fsl_adc16.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_ADC16_BASE ADC0
#define DEMO_ADC16_CHANNEL_GROUP 0U
#define DEMO_ADC16_USER_CHANNEL 26U /*PTE20, ADC0_SE0 */
/* Task priorities. */
#define hello_task_PRIORITY (configMAX_PRIORITIES - 1)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void hello_task(void *pvParameters);

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application entry point.
 */

adc16_config_t adc16ConfigStruct;
adc16_channel_config_t adc16ChannelConfigStruct;

int main(void)
{
    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();


    //ADC Initialization
    ADC16_GetDefaultConfig(&adc16ConfigStruct);
#ifdef BOARD_ADC_USE_ALT_VREF
    adc16ConfigStruct.referenceVoltageSource = kADC16_ReferenceVoltageSourceValt;
#endif
    ADC16_Init(DEMO_ADC16_BASE, &adc16ConfigStruct);
    ADC16_EnableHardwareTrigger(DEMO_ADC16_BASE, false); /* Make sure the software trigger is used. */
#if defined(FSL_FEATURE_ADC16_HAS_CALIBRATION) && FSL_FEATURE_ADC16_HAS_CALIBRATION
    if (kStatus_Success == ADC16_DoAutoCalibration(DEMO_ADC16_BASE))
    {
    	PRINTF("ADC16_DoAutoCalibration() Done.\r\n");
    }
    else
    {
    	PRINTF("ADC16_DoAutoCalibration() Failed.\r\n");
    }
#endif /* FSL_FEATURE_ADC16_HAS_CALIBRATION */

    adc16ChannelConfigStruct.channelNumber = DEMO_ADC16_USER_CHANNEL;
    adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = false;
#if defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE
    adc16ChannelConfigStruct.enableDifferentialConversion = false;
#endif /* FSL_FEATURE_ADC16_HAS_DIFF_MODE */

    xTaskCreate(hello_task, "Hello_task", configMINIMAL_STACK_SIZE + 10, NULL, hello_task_PRIORITY, NULL);
    vTaskStartScheduler();
    for (;;)
        ;
}

/*!
 * @brief Task responsible for printing of "Hello world." message.
 */
static void hello_task(void *pvParameters)
{
	uint32_t temp = 0;
	float temp_float = 0;
    for (;;)
    {
//        PRINTF("Hello world.\r\n");
//        vTaskSuspend(NULL);

    	ADC16_SetChannelConfig(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP, &adc16ChannelConfigStruct);
    	while (0U == (kADC16_ChannelConversionDoneFlag &
    			ADC16_GetChannelStatusFlags(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP)))
    	{
    	}
    	//PRINTF("ADC Value: %d\r\n", ADC16_GetChannelConversionValue(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP));
    	temp = ADC16_GetChannelConversionValue(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP);
    	//temp_float = temp * 0.000732;//0.0029296875; //Voltage conversion 3.0V/1024 = 0.0029296875;
    	float v2 = temp*3300.0f/4096;
    	int32_t v25 = 716;
    	int32_t m = 1620;
    	temp_float = 25.0f-(v2-v25)*1000.0f/m;
    	temp = temp_float/1;
//    	if(temp_float >= 0.7012) //Check for Hot or Cold Slope
//    	{
//    		temp_float = 25 -((temp_float - 0.7012)/0.001646); //Cold Slope
//    	}
//    	else
//    	{
//    		temp_float = 25 -((temp_float - 0.7012)/0.001749); //Hot Slope
//    	}

//    	PRINTF("ADC Value: %f\r\n", temp_float);
    	char d, u, dec;
    	d = temp_float*10/100;
    	u = (temp_float*10 - d*100) /10;
    	dec = (temp_float*10 - d*100 - u*10) /1;
    	PRINTF("ADC Value: %d%d,%d\r\n", d,u,dec);

    	const TickType_t Delay = 500 *configTICK_RATE_HZ/1000;
    	vTaskDelay(Delay);
    }
}
