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

/* Declare a variable of type xQueueHandle. This is used to store the reference
to the queue that is accessed by all three tasks. */
QueueHandle_t xQueue;

int main(void)
{
    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
//    xTaskCreate(hello_task, "Hello_task", configMINIMAL_STACK_SIZE + 10, NULL, hello_task_PRIORITY, NULL);

    /* The queue is created to hold a maximum of 5 values, each of which is
    large enough to hold a variable of type long. */
    xQueue = xQueueCreate( 5, sizeof( long ) );

    if( xQueue != NULL )
    {
    	/* Create two instances of the task that will send to the queue. The task
    parameter is used to pass the value that the task will write to the queue,
    so one task will continuously write 100 to the queue while the other task
    will continuously write 200 to the queue. Both tasks are created at
    priority 1. */
    	xTaskCreate( vSenderTask, "Sender1", 100, ( void * ) 100, 1, NULL );
    	xTaskCreate( vSenderTask, "Sender2", 100, ( void * ) 200, 1, NULL );
    	/* Create the task that will read from the queue. The task is created with
    priority 2, so above the priority of the sender tasks. */
    	xTaskCreate( vReceiverTask, "Receiver", 100, NULL, 2, NULL );
    	/* Start the scheduler so the created tasks start executing. */
    	vTaskStartScheduler();
    }
    else
    {
    	/* The queue could not be created. */
    }
//    vTaskStartScheduler();
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
static void vSenderTask( void *pvParameters )
{
	long lValueToSend;
	portBASE_TYPE xStatus;
	/* Two instances of this task are created so the value that is sent to the
queue is passed in via the task parameter - this way each instance can use
a different value. The queue was created to hold values of type long,
so cast the parameter to the required type. */
	lValueToSend = ( long ) pvParameters;
	/* As per most tasks, this task is implemented within an infinite loop. */
	for( ;; )
	{
		/* Send the value to the queue.
The first parameter is the queue to which data is being sent. The
queue was created before the scheduler was started, so before this task
started to execute.
The second parameter is the address of the data to be sent, in this case
the address of lValueToSend.
The third parameter is the Block time – the time the task should be kept
in the Blocked state to wait for space to become available on the queue
should the queue already be full. In this case a block time is not
specified because the queue should never contain more than one item and
therefore never be full. */
		xStatus = xQueueSendToBack( xQueue, &lValueToSend, 0 );
		if( xStatus != pdPASS )
		{
			/* The send operation could not complete because the queue was full -
this must be an error as the queue should never contain more than

one item! */
			PRINTF( "Could not send to the queue.\r\n" );
		}
		/* Allow the other sender task to execute. taskYIELD() informs the
scheduler that a switch to another task should occur now rather than
keeping this task in the Running state until the end of the current time
slice. */
		taskYIELD();
	}
}

static void vReceiverTask( void *pvParameters )
{
	/* Declare the variable that will hold the values received from the queue. */
	long lReceivedValue;
	portBASE_TYPE xStatus;
	const TickType_t xTicksToWait = 100 *configTICK_RATE_HZ/1000;
	/* This task is also defined within an infinite loop. */
	for( ;; )
	{
		/* This call should always find the queue empty because this task will
immediately remove any data that is written to the queue. */
		if( uxQueueMessagesWaiting( xQueue ) != 0 )
		{
			PRINTF( "Queue should have been empty!\r\n" );
		}
		/* Receive data from the queue.
The first parameter is the queue from which data is to be received. The
queue is created before the scheduler is started, and therefore before this
task runs for the first time.
The second parameter is the buffer into which the received data will be
placed. In this case the buffer is simply the address of a variable that
has the required size to hold the received data.
The last parameter is the block time – the maximum amount of time that the
task should remain in the Blocked state to wait for data to be available
should the queue already be empty. In this case the constant
portTICK_RATE_MS is used to convert 100 milliseconds to a time specified in
ticks. */
		xStatus = xQueueReceive( xQueue, &lReceivedValue, xTicksToWait );
		if( xStatus == pdPASS )
		{
			/* Data was successfully received from the queue, print out the received
value. */
			PRINTF( "Received = %d\n\r", lReceivedValue );
		}
		else
		{
			/* Data was not received from the queue even after waiting for 100ms.
This must be an error as the sending tasks are free running and will be
continuously writing to the queue. */
			PRINTF( "Could not receive from the queue.\r\n" );
		}
	}
}
