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
void vTask1( void *pvParameters );
void vTask2( void *pvParameters );
void vTaskFunction(void *pvParameters);

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application entry point.
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    //xTaskCreate(hello_task, "Hello_task", configMINIMAL_STACK_SIZE + 10, NULL, hello_task_PRIORITY, NULL);

    /* Create one of the two tasks. Note that a real application should check
    the return value of the xTaskCreate() call to ensure the task was created
    successfully. */
    xTaskCreate( vTask1, "Task 1", 100, NULL, 1, NULL);
    /* Create the other task in exactly the same way and at the same priority. */
    xTaskCreate( vTaskFunction, "TaskFunction", 100, "TaskFunction is running\r\n", 1, NULL);

    vTaskStartScheduler();
    for (;;);
}

void vTask1(void *pvParameters) {
	const char *pcTaskName = "Task 1 is running\r\n";
	volatile unsigned long ul;
	/* If this task code is executing then the scheduler must already have been started.*/
	xTaskCreate(vTask2, "Task 2", 100, NULL, 1, NULL);
	for (;;) {
		/* Print out the name of this task. */
		PRINTF(pcTaskName);
		/* Delay for a period. */
		for (ul = 0; ul < mainDELAY_LOOP_COUNT; ul++) {
			/* This loop is just a very crude delay implementation. There is nothing to do in here.*/
		}
	}
}

void vTask2( void *pvParameters )
{
	const char *pcTaskName = "Task 2 is running\r\n";
	volatile unsigned long ul;
	/* As per most tasks, this task is implemented in an infinite loop. */
	for( ;; )
	{
		/* Print out the name of this task. */
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

void vTaskFunction(void *pvParameters) {
	char *pcTaskName;
	volatile unsigned long ul;
	/* The string to print out is passed in via the parameter. Cast this to a character pointer. */
	pcTaskName = (char*) pvParameters;
	/* As per most tasks, this task is implemented in an infinite loop. */
	for (;;) {
		/* Print out the name of this task. */
		PRINTF(pcTaskName);
		/* Delay for a period. */
		for (ul = 0; ul < mainDELAY_LOOP_COUNT; ul++) {/* This loop is just a very crude delay implementation. */
		}
	}
}
