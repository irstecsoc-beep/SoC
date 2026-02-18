/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

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
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void vPeriodicTask( void *pvParameters );
static void vHandlerTask( void *pvParameters );
/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool pitIsrFlag = false;
SemaphoreHandle_t xBinarySemaphore = NULL;
TaskHandle_t * periodic_task;
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
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(500000U, PIT_SOURCE_CLOCK));

    /* Enable timer interrupts for channel 0 */
    PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);

    /* Enable at the NVIC */
    EnableIRQ(PIT_IRQ_ID);

    /* Before a semaphore is used it must be explicitly created. In this example
    a binary semaphore is created. */
    xBinarySemaphore = xSemaphoreCreateBinary();
    /* Install the interrupt handler. */
    //_dos_setvect( 0x82, vExampleInterruptHandler );
    /* Check the semaphore was created successfully. */
    if( xBinarySemaphore != NULL )
    {
    	/* Create the 'handler' task. This is the task that will be synchronized
    with the interrupt. The handler task is created with a high priority to
    ensure it runs immediately after the interrupt exits. In this case a
    priority of 3 is chosen. */
    	xTaskCreate( vHandlerTask, "Handler", 100, NULL, 3, NULL );
    	/* Create the task that will periodically generate a software interrupt.
    This is created with a priority below the handler task to ensure it will
    get pre-empted each time the handler task exits the Blocked state. */
    	xTaskCreate( vPeriodicTask, "Periodic", 100, NULL, 1, &periodic_task );

    	/* Start the scheduler so the created tasks start executing. */
    	vTaskStartScheduler();
    }
    for (;;)
        ;
}

static void vPeriodicTask( void *pvParameters )
{
	for( ;; )
	{
		/* This task is just used to 'simulate' an interrupt by generating a
software interrupt every 500ms. */
		/* Generate the interrupt, printing a message both before and after
so the sequence of execution is evident from the output produced when
the example is executed. */
		PRINTF( "Periodic task - About to generate an interrupt.\r\n" );
		PIT_StartTimer(PIT, kPIT_Chnl_0);
		vTaskSuspend(periodic_task);
		PRINTF( "Periodic task - Interrupt generated.\r\n\r\n\r\n" );
	}
}

static void vHandlerTask( void *pvParameters )
{
	/* As per most tasks, this task is implemented within an infinite loop. */
	for( ;; )
	{
		/* Use the semaphore to wait for an event. The semaphore was created
before the scheduler was started so before this task ran for the first
time. The task blocks indefinitely so the function call will only
return once the semaphore has been successfully taken (obtained). There
is therefore no need to check the function return value. */
		xSemaphoreTake( xBinarySemaphore, portMAX_DELAY );
		/* To get here the event must have occurred. Process the event. In this
case processing is simply a matter of printing out a message. */
		PRINTF( "Handler task - Processing event.\r\n" );
		vTaskResume(periodic_task);
	}
}

void PIT_LED_HANDLER(void)
{
	static portBASE_TYPE xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;
	/* 'Give' the semaphore to unblock the task. */
	xSemaphoreGiveFromISR( xBinarySemaphore, &xHigherPriorityTaskWoken );
	if( xHigherPriorityTaskWoken == pdTRUE )
	{
		/* Giving the semaphore unblocked a task, and the priority of the
    unblocked task is higher than the currently running task - force
    a context switch to ensure that the interrupt returns directly to
    the unblocked (higher priority) task.
    NOTE: The actual macro to use to force a context switch from an
    ISR is dependent on the port. This is the correct macro for the
    Open Watcom DOS port. Other ports may require different syntax.
    Refer to the examples provided for the port being used to determine
    the syntax required. */
		//		portSWITCH_CONTEXT();
		/* Clear interrupt flag.*/
		PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
		pitIsrFlag = true;
	}
}

