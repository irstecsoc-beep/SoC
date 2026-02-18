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
//    xTaskCreate(hello_task, "Hello_task", configMINIMAL_STACK_SIZE + 10, NULL, hello_task_PRIORITY, NULL);
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
	/* The string to print out is passed in via the parameter. Cast this to a
character pointer. */
	pcTaskName = ( char * ) pvParameters;
	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		/* Print out the name of this task. */
		PRINTF( pcTaskName );
		LED_RED_ON();
		vTaskDelay(500*configTICK_RATE_HZ/1000); //500 ms
		LED_RED_OFF();
		vTaskDelay(500*configTICK_RATE_HZ/1000); //500 ms
	}
}

void vTask2Function( void *pvParameters )
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
		LED_GREEN_ON();
		vTaskDelay(500*configTICK_RATE_HZ/1000); //500 ms
		LED_GREEN_OFF();
		vTaskDelay(500*configTICK_RATE_HZ/1000); //500 ms
	}
}
