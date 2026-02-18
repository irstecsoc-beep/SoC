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
#define mainDELAY_LOOP_COUNT 10000000
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
//static void hello_task(void *pvParameters);
void vTaskFunction( void *pvParameters );

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application entry point.
 */

/* Define the strings that will be passed in as the task parameters. These are
defined const and not on the stack to ensure they remain valid when the tasks are
executing. */
static const char *pcTextForTask1 = "Task 1 is running\r\n";
static const char *pcTextForTask2 = "Task 2 is running\t\r\n";

int main(void)
{
    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    //    xTaskCreate(hello_task, "Hello_task", configMINIMAL_STACK_SIZE + 10, NULL, hello_task_PRIORITY, NULL);
    /* Create one of the two tasks. */
    xTaskCreate( vTaskFunction, "Task 1", 1000, (void*)pcTextForTask1, 1, NULL );
    /* Create the other task in exactly the same way. Note this time that multiple
    tasks are being created from the SAME task implementation (vTaskFunction). Only
    the value passed in the parameter is different. Two instances of the same
    task are being created. */
    xTaskCreate( vTaskFunction, "Task 2", 1000, (void*)pcTextForTask2, 1, NULL );

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
void vTaskFunction( void *pvParameters )
{
	char *pcTaskName;
	volatile unsigned long ul;
	/* The string to print out is passed in via the parameter. Cast this to a
character pointer. */
	pcTaskName = ( char * ) pvParameters;
	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		/* Print out the name of this task. */
//		vPrintString( pcTaskName );
		PRINTF(pcTaskName);
		LED_GREEN_ON();
		for( ul = 0; ul < mainDELAY_LOOP_COUNT; ul++ )
		{
		}
//		vTaskDelay(500*configTICK_RATE_HZ/1000); //500 ms
//		LED_GREEN_OFF();
//		for( ul = 0; ul < mainDELAY_LOOP_COUNT; ul++ )
//		{
//		}
	}
}
