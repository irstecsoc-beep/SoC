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
void vTaskFunction( void *pvParameters );

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application entry point.
 */
static const char *pcTextForTask1 = "Task 1 is running\r\n";
/* Declare a variable that will be incremented by the hook function. */
unsigned long ulIdleCycleCount = 0UL;

int main(void)
{
    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    xTaskCreate( vTaskFunction, "Task 1", 100, (void*)pcTextForTask1, 1, NULL );

    vTaskStartScheduler();
    for (;;)
        ;
}

/* Idle hook functions MUST be called vApplicationIdleHook(), take no parameters,
and return void. */
void vApplicationIdleHook( void )
{
/* This hook function does nothing but increment a counter. */
ulIdleCycleCount++;
}

void vTaskFunction( void *pvParameters )
{
	char *pcTaskName;
	/* The string to print out is passed in via the parameter. Cast this to a
character pointer. */
	pcTaskName = ( char * ) pvParameters;
	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		/* Print out the name of this task. */
		PRINTF( pcTaskName );
		PRINTF("ulIdleCycleCount = %d\n\r", ulIdleCycleCount );
		/* Delay for a period for 250 milliseconds. */
		vTaskDelay(250*configTICK_RATE_HZ/1000); //250 ms
	}
}

