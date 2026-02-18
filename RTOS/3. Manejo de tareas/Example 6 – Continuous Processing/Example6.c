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
#define mainDELAY_LOOP_COUNT 100000
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
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

int main(void)
{
    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    xTaskCreate( vContinuousProcessingTask1, "ContinuosProcessingTask 1", 100, (void*)pcTextForTask1, 1, NULL );
    xTaskCreate( vContinuousProcessingTask2, "ContinuosProcessingTask 2", 100, (void*)pcTextForTask2, 1, NULL );

    xTaskCreate( vPeriodicTask, "PeriodicTask", 100, NULL, 2, NULL );

    vTaskStartScheduler();
    for (;;)
        ;
}

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
		for( ul = 0; ul < mainDELAY_LOOP_COUNT; ul++ )
		{
		}
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
		for( ul = 0; ul < mainDELAY_LOOP_COUNT; ul++ )
		{
		}
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
		for( ul = 0; ul < mainDELAY_LOOP_COUNT; ul++ )
		{
		}
		vTaskDelayUntil( &xLastWakeTime, ( 1000 / portTICK_PERIOD_MS ) );
	}
}
