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
#include "fsl_pit.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define PIT_LED_HANDLER PIT_IRQHandler
#define PIT_IRQ_ID PIT_IRQn
/* Get source clock for PIT driver */
#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)
/* Task priorities. */
//#define hello_task_PRIORITY (configMAX_PRIORITIES - 1)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
//static void hello_task(void *pvParameters);
static void vIntegerGenerator( void *pvParameters );
static void vStringPrinter( void *pvParameters );
/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool pitIsrFlag = false;
//SemaphoreHandle_t xCountingSemaphore = NULL;
TaskHandle_t * IntegerGenerator;
QueueHandle_t xIntegerQueue = NULL;
QueueHandle_t xStringQueue = NULL;
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application entry point.
 */
int main(void)
{
	/* Structure of initialize PIT */
	pit_config_t pitConfig;

	/* Init board hardware. */
	BOARD_InitPins();
	BOARD_BootClockRUN();
	BOARD_InitDebugConsole();

	/*
	 * pitConfig.enableRunInDebug = false;
	 */
	PIT_GetDefaultConfig(&pitConfig);

	/* Init pit module */
	PIT_Init(PIT, &pitConfig);

	/* Set timer period for channel 0 */
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(200000U, PIT_SOURCE_CLOCK));

	/* Enable timer interrupts for channel 0 */
	PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);

	/* Enable at the NVIC */
	EnableIRQ(PIT_IRQ_ID);

	/* Before a queue can be used it must first be created. Create both queues
	used by this example. One queue can hold variables of type unsigned long,
	the other queue can hold variables of type char*. Both queues can hold a
	maximum of 10 items. A real application should check the return values to
	ensure the queues have been successfully created. */
	xIntegerQueue = xQueueCreate( 10, sizeof( unsigned long ) );
	xStringQueue = xQueueCreate( 10, sizeof( char * ) );

	//    xTaskCreate(hello_task, "Hello_task", configMINIMAL_STACK_SIZE + 10, NULL, hello_task_PRIORITY, NULL);
//	vTaskStartScheduler();

	/* Install the interrupt handler. */
//	_dos_setvect( 0x82, vExampleInterruptHandler );
	/* Create the task that uses a queue to pass integers to the interrupt service
	routine. The task is created at priority 1. */
	xTaskCreate( vIntegerGenerator, "IntGen", 1000, NULL, 1, &IntegerGenerator );
	/* Create the task that prints out the strings sent to it from the interrupt
	service routine. This task is created at the higher priority of 2. */
	xTaskCreate( vStringPrinter, "String", 1000, NULL, 2, NULL );
	/* Start the scheduler so the created tasks start executing. */
	vTaskStartScheduler();
	/* If all is well then main() will never reach here as the scheduler will
	now be running the tasks. If main() does reach here then it is likely that
	there was insufficient heap memory available for the idle task to be created.
	CHAPTER 5 provides more information on memory management. */
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

static void vIntegerGenerator( void *pvParameters )
{
	//portTickType xLastExecutionTime;
	unsigned portLONG ulValueToSend = 0;
	int i;
	/* Initialize the variable used by the call to vTaskDelayUntil(). */
	//xLastExecutionTime = xTaskGetTickCount();
	for( ;; )
	{
		/* This is a periodic task. Block until it is time to run again.
The task will execute every 200ms. */
		//vTaskDelayUntil( &xLastExecutionTime, 200 / portTICK_RATE_MS );
		/* Send an incrementing number to the queue five times. The values will
be read from the queue by the interrupt service routine. The interrupt
service routine always empties the queue so this task is guaranteed to be
able to write all five values, so a block time is not required. */
		for( i = 0; i < 5; i++ )
		{
			xQueueSendToBack( xIntegerQueue, &ulValueToSend, 0 );
			ulValueToSend++;
		}
		/* Force an interrupt so the interrupt service routine can read the
values from the queue. */
		PRINTF( "Generator task - About to generate an interrupt.\r\n" );
		//		__asm{ int 0x82 } /* This line generates the interrupt. */
		PIT_StartTimer(PIT, kPIT_Chnl_0);
		vTaskSuspend(IntegerGenerator);
		PRINTF( "Generator task - Interrupt generated.\r\n\r\n\r\n" );
	}
}

static void vStringPrinter( void *pvParameters )
{
	char *pcString;
	for( ;; )
	{
		/* Block on the queue to wait for data to arrive. */
		xQueueReceive( xStringQueue, &pcString, portMAX_DELAY );
		/* Print out the string received. */
		PRINTF( pcString );
	}
}

void PIT_LED_HANDLER(void)
{
	static portBASE_TYPE xHigherPriorityTaskWoken;
	static unsigned long ulReceivedNumber;
	/* The strings are declared static const to ensure they are not allocated
	on the stack of the ISR, and exist even when the ISR is not executing. */
	static const char *pcStrings[] =
	{
			"String 0\r\n",
			"String 1\r\n",
			"String 2\r\n",
			"String 3\r\n"
	};
	xHigherPriorityTaskWoken = pdFALSE;
	/* Loop until the queue is empty. */
	while( xQueueReceiveFromISR( xIntegerQueue,
			&ulReceivedNumber,
			&xHigherPriorityTaskWoken ) != errQUEUE_EMPTY )
	{
		/* Truncate the received value to the last two bits (values 0 to 3 inc.), then
	send a pointer to the string that corresponds to the truncated value to a
	different queue. */
		ulReceivedNumber &= 0x03;
		xQueueSendToBackFromISR( xStringQueue,
				&pcStrings[ ulReceivedNumber ],
				&xHigherPriorityTaskWoken );
	}
	/* Did receiving on a queue or sending on a queue unblock a task that has a
	priority higher than the currently executing task? If so, force a context
	switch here. */
	if( xHigherPriorityTaskWoken == pdTRUE )
	{
		/* NOTE: The actual macro to use to force a context switch from an ISR is
	dependent on the port. This is the correct macro for the Open Watcom DOS
	port. Other ports may require different syntax. Refer to the examples
	provided for the port being used to determine the syntax required. */
		//		portSWITCH_CONTEXT();
		/* Clear interrupt flag.*/
		PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
		pitIsrFlag = true;
		xTaskResumeFromISR(IntegerGenerator);
	}
}

//static void __interrupt __far vExampleInterruptHandler( void )
//{
//	static portBASE_TYPE xHigherPriorityTaskWoken;
//	static unsigned long ulReceivedNumber;
//	/* The strings are declared static const to ensure they are not allocated
//on the stack of the ISR, and exist even when the ISR is not executing. */
//	static const char *pcStrings[] =
//	{
//			"String 0\r\n",
//			"String 1\r\n",
//			"String 2\r\n",
//			"String 3\r\n"
//	};
//	xHigherPriorityTaskWoken = pdFALSE;
//	/* Loop until the queue is empty. */
//	while( xQueueReceiveFromISR( xIntegerQueue,
//			&ulReceivedNumber,
//			&xHigherPriorityTaskWoken ) != errQUEUE_EMPTY )
//	{
//		/* Truncate the received value to the last two bits (values 0 to 3 inc.), then
//send a pointer to the string that corresponds to the truncated value to a
//different queue. */
//		ulReceivedNumber &= 0x03;
//		xQueueSendToBackFromISR( xStringQueue,
//				&pcStrings[ ulReceivedNumber ],
//				&xHigherPriorityTaskWoken );
//	}
//	/* Did receiving on a queue or sending on a queue unblock a task that has a
//priority higher than the currently executing task? If so, force a context
//switch here. */
//	if( xHigherPriorityTaskWoken == pdTRUE )
//	{
//		/* NOTE: The actual macro to use to force a context switch from an ISR is
//dependent on the port. This is the correct macro for the Open Watcom DOS
//port. Other ports may require different syntax. Refer to the examples
//provided for the port being used to determine the syntax required. */
//		portSWITCH_CONTEXT();
//	}
//}
