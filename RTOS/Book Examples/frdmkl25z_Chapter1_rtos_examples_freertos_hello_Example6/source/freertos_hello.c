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
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Task priorities. */
//#define hello_task_PRIORITY (configMAX_PRIORITIES - 1)
#define mainDELAY_LOOP_COUNT 100000
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
//static void hello_task(void *pvParameters);
void vContinuousProcessingTask1( void *pvParameters );
void vContinuousProcessingTask2( void *pvParameters );
void vPeriodicTask( void *pvParameters );
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application entry point.
 */
static const char *pcTextForTask1 = "Continuous task 1 running\t\r\n";
static const char *pcTextForTask2 = "Continuous task 2 running\t\r\n";
BaseType_t yoyi;

int main(void)
{
    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
//    xTaskCreate(hello_task, "Hello_task", configMINIMAL_STACK_SIZE + 10, NULL, hello_task_PRIORITY, NULL);
    xTaskCreate( vContinuousProcessingTask1, "ContinuosProcessingTask 1", 100, (void*)pcTextForTask1, 1, NULL );
    xTaskCreate( vContinuousProcessingTask2, "ContinuosProcessingTask 2", 100, (void*)pcTextForTask2, 1, NULL );

    xTaskCreate( vPeriodicTask, "PeriodicTask", 100, NULL, 2, NULL );

    vTaskStartScheduler();
    for (;;)
        ;
}

/*!
 * @brief Task responsible for printing of "Hello world." message.
 */
//static void hello_task(void *pvParameters)
//{
//    for (;;)
//    {
//        PRINTF("Hello world.\r\n");
//        vTaskSuspend(NULL);
//    }
//}
void vContinuousProcessingTask1( void *pvParameters )
{
	char *pcTaskName;
	/* The string to print out is passed in via the parameter. Cast this to a
character pointer. */
	pcTaskName = ( char * ) pvParameters;
	volatile unsigned long ul;
	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		/* Print out the name of this task. This task just does this repeatedly
without ever blocking or delaying. */
		PRINTF( pcTaskName );
		LED_GREEN_OFF();
		LED_BLUE_OFF();
		LED_RED_ON();
		for( ul = 0; ul < mainDELAY_LOOP_COUNT; ul++ )
		{
		}
//		vTaskDelay(200 / portTICK_PERIOD_MS);
//		LED_RED_OFF();
//		vTaskDelay(200 / portTICK_PERIOD_MS);
	}
}

void vContinuousProcessingTask2( void *pvParameters )
{
	char *pcTaskName;
	/* The string to print out is passed in via the parameter. Cast this to a
character pointer. */
	pcTaskName = ( char * ) pvParameters;
	volatile unsigned long ul;
	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		/* Print out the name of this task. This task just does this repeatedly
without ever blocking or delaying. */
		PRINTF( pcTaskName );
		LED_RED_OFF();
		LED_BLUE_OFF();
		LED_GREEN_ON();
		for( ul = 0; ul < mainDELAY_LOOP_COUNT; ul++ )
		{
		}
//		vTaskDelay(200 / portTICK_PERIOD_MS);
//		LED_GREEN_OFF();
//		vTaskDelay(200 / portTICK_PERIOD_MS);
	}
}

void vPeriodicTask( void *pvParameters )
{
	TickType_t xLastWakeTime;
	volatile unsigned long ul;
	/* The xLastWakeTime variable needs to be initialized with the current tick
count. Note that this is the only time the variable is explicitly written to.
After this xLastWakeTime is managed automatically by the vTaskDelayUntil()
API function. */
	xLastWakeTime = xTaskGetTickCount();
	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		/* Print out the name of this task. */
		PRINTF( "Periodic task is running\t\n\r" );
		LED_RED_OFF();
		LED_GREEN_OFF();
		LED_BLUE_ON();
		for( ul = 0; ul < mainDELAY_LOOP_COUNT; ul++ )
		{
		}
//		vTaskDelay(100 / portTICK_PERIOD_MS);
//		LED_BLUE_OFF();
//		vTaskDelay(100 / portTICK_PERIOD_MS);
		/* The task should execute every 10 milliseconds exactly. */
		vTaskDelayUntil( &xLastWakeTime, ( 1000 / portTICK_PERIOD_MS ) );
	}
}
