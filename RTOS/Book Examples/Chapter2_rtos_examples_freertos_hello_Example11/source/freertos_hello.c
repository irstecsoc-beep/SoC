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
#define mainSENDER_1 1
#define mainSENDER_2 2
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
//static void hello_task(void *pvParameters);
static void vSenderTask( void *pvParameters );
static void vReceiverTask( void *pvParameters );
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application entry point.
 */

/* Define the structure type that will be passed on the queue. */
typedef struct
{
	unsigned char ucValue;
	unsigned char ucSource;
} xData;
/* Declare two variables of type xData that will be passed on the queue. */
static const xData xStructsToSend[ 2 ] =
{
		{ 100, mainSENDER_1 }, /* Used by Sender1. */
		{ 200, mainSENDER_2 } /* Used by Sender2. */
};
QueueHandle_t xQueue;

int main(void)
{
	/* Init board hardware. */
	BOARD_InitPins();
	BOARD_BootClockRUN();
	BOARD_InitDebugConsole();
	//    xTaskCreate(hello_task, "Hello_task", configMINIMAL_STACK_SIZE + 10, NULL, hello_task_PRIORITY, NULL);

	/* The queue is created to hold a maximum of 3 structures of type xData. */
	xQueue = xQueueCreate( 3, sizeof( xData ) );
	if( xQueue != NULL )
	{
		/* Create two instances of the task that will write to the queue. The
    parameter is used to pass the structure that the task will write to the
    queue, so one task will continuously send xStructsToSend[ 0 ] to the queue
    while the other task will continuously send xStructsToSend[ 1 ]. Both
    tasks are created at priority 2 which is above the priority of the receiver. */
		xTaskCreate( vSenderTask, "Sender1", 100, &( xStructsToSend[ 0 ] ), 2, NULL );
		xTaskCreate( vSenderTask, "Sender2", 100, &( xStructsToSend[ 1 ] ), 2, NULL );
		/* Create the task that will read from the queue. The task is created with
    priority 1, so below the priority of the sender tasks. */
		xTaskCreate( vReceiverTask, "Receiver", 100, NULL, 1, NULL );
		/* Start the scheduler so the created tasks start executing. */
		vTaskStartScheduler();
	}
	else
	{
		/* The queue could not be created. */
	}
    /* If all is well then main() will never reach here as the scheduler will
    now be running the tasks. If main() does reach here then it is likely that
    there was insufficient heap memory available for the idle task to be created.
    CHAPTER 5 provides more information on memory management. */

//    vTaskStartScheduler();
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
static void vSenderTask( void *pvParameters )
{
	portBASE_TYPE xStatus;
	const TickType_t xTicksToWait = 100 *configTICK_RATE_HZ/1000;
	/* As per most tasks, this task is implemented within an infinite loop. */
	for( ;; )
	{
		/* Send to the queue.
The second parameter is the address of the structure being sent. The
address is passed in as the task parameter so pvParameters is used
directly.
The third parameter is the Block time - the time the task should be kept
in the Blocked state to wait for space to become available on the queue
if the queue is already full. A block time is specified because the
sending tasks have a higher priority than the receiving task so the queue
is expected to become full. The receiving task will on execute and remove
items from the queue when both sending tasks are in the Blocked state. */
		xStatus = xQueueSendToBack( xQueue, pvParameters, xTicksToWait );
		if( xStatus != pdPASS )
		{
			/* The send operation could not complete, even after waiting for 100ms.
This must be an error as the receiving task should make space in the
queue as soon as both sending tasks are in the Blocked state. */
			PRINTF( "Could not send to the queue.\r\n" );
		}
		/* Allow the other sender task to execute. */
		taskYIELD();
	}
}

static void vReceiverTask( void *pvParameters )
{
	/* Declare the structure that will hold the values received from the queue. */
	xData xReceivedStructure;
	portBASE_TYPE xStatus;
	/* This task is also defined within an infinite loop. */
	for( ;; )
	{
		/* Because it has the lowest priority this task will only run when the
sending tasks are in the Blocked state. The sending tasks will only enter
the Blocked state when the queue is full so this task always expects the
number of items in the queue to be equal to the queue length – 3 in this
case. */
		if( uxQueueMessagesWaiting( xQueue ) != 3 )
		{
			PRINTF( "Queue should have been full!\r\n" );
		}
		/* Receive from the queue.
The second parameter is the buffer into which the received data will be
placed. In this case the buffer is simply the address of a variable that
has the required size to hold the received structure.
The last parameter is the block time - the maximum amount of time that the
task will remain in the Blocked state to wait for data to be available
if the queue is already empty. In this case a block time is not necessary
because this task will only run when the queue is full. */
		xStatus = xQueueReceive( xQueue, &xReceivedStructure, 0 );
		if( xStatus == pdPASS )
		{
			/* Data was successfully received from the queue, print out the received
value and the source of the value. */
			if( xReceivedStructure.ucSource == mainSENDER_1 )
			{
				PRINTF( "From Sender 1 = %d\n\r", xReceivedStructure.ucValue );
			}
			else
			{
				PRINTF( "From Sender 2 = %d\n\r", xReceivedStructure.ucValue );
			}
		}
		else
		{
			/* Nothing was received from the queue. This must be an error
as this task should only run when the queue is full. */
			PRINTF( "Could not receive from the queue.\r\n" );
		}
	}
}
