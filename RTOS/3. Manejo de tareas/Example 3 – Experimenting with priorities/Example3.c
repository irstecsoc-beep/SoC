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

#define mainDELAY_LOOP_COUNT 1000000
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
    /* Create the first task at priority 1. The priority is the second to last
    parameter. */
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
	volatile unsigned long ul;
	/* The string to print out is passed in via the parameter. Cast this to a
character pointer. */
	pcTaskName = ( char * ) pvParameters;
	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		PRINTF(pcTaskName);
		LED_GREEN_ON();
		for( ul = 0; ul < mainDELAY_LOOP_COUNT; ul++ )
		{
		}
		LED_GREEN_OFF();
		for( ul = 0; ul < mainDELAY_LOOP_COUNT; ul++ )
		{
		}
	}
}

void vTask2Function( void *pvParameters )
{
	char *pcTaskName;
	volatile unsigned long ul;
	/* The string to print out is passed in via the parameter. Cast this to a
character pointer. */
	pcTaskName = ( char * ) pvParameters;
	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		PRINTF(pcTaskName);
		LED_RED_ON();
		for( ul = 0; ul < mainDELAY_LOOP_COUNT; ul++ )
		{
		}
		LED_RED_OFF();
		for( ul = 0; ul < mainDELAY_LOOP_COUNT; ul++ )
		{
		}
	}
}
