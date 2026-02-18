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
 * Prototypes
 ******************************************************************************/
void vTask1Function( void *pvParameters );
void vTask2Function( void *pvParameters );

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application entry point.
 */
static const char *pcTextForTask1 = "Task 1 is running\r\n";
static const char *pcTextForTask2 = "Task 2 is running\t\r\n";

int main(void)
{
    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    xTaskCreate( vTask1Function, "Task 1", 1000, (void*)pcTextForTask1, 1, NULL );
    /* Create the second task at priority 2. */
    xTaskCreate( vTask2Function, "Task 2", 1000, (void*)pcTextForTask2, 2, NULL );
    vTaskStartScheduler();
    for (;;)
        ;
}

void vTask1Function( void *pvParameters )
{
	char *pcTaskName;
	TickType_t xLastWakeTime;
	pcTaskName = ( char * ) pvParameters;
	xLastWakeTime = xTaskGetTickCount();

	for( ;; )
	{
		/* Print out the name of this task. */
		PRINTF( pcTaskName );
		LED_RED_ON();
		vTaskDelayUntil( &xLastWakeTime, ( 500*configTICK_RATE_HZ/1000 ) );
	}
}

void vTask2Function( void *pvParameters )
{
	char *pcTaskName;
	TickType_t xLastWakeTime;
	pcTaskName = ( char * ) pvParameters;
	xLastWakeTime = xTaskGetTickCount();

	for( ;; )
	{
		/* Print out the name of this task. */
		PRINTF( pcTaskName );
		LED_GREEN_ON();
		vTaskDelayUntil( &xLastWakeTime, ( 500*configTICK_RATE_HZ/1000 ) );
	}
}
