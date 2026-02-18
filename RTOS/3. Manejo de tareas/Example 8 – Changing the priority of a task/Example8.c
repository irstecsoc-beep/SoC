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
#define hello_task_PRIORITY (configMAX_PRIORITIES - 1)
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
TaskHandle_t xTask1Handle, xTask2Handle;

int main(void)
{
    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    xTaskCreate( vTask1Function, "Task 1", 1000, (void*)pcTextForTask1, 2, NULL );
    /* Create the second task at priority 2. */
    xTaskCreate( vTask2Function, "Task 2", 1000, (void*)pcTextForTask2, 1, &xTask2Handle );
    vTaskStartScheduler();
    for (;;)
        ;
}

void vTask1Function( void *pvParameters )
{
	unsigned portBASE_TYPE uxPriority;
	/* This task will always run before Task2 as it is created with the higher
priority. Neither Task1 nor Task2 ever block so both will always be in either
the Running or the Ready state.
Query the priority at which this task is running - passing in NULL means
"return my priority". */
	uxPriority = uxTaskPriorityGet( NULL );
	for( ;; )
	{
		/* Print out the name of this task. */
		PRINTF( "Task1 is running\r\n" );
		/* Setting the Task2 priority above the Task1 priority will cause
Task2 to immediately start running (as then Task2 will have the higher
priority of the two created tasks). Note the use of the handle to task
2 (xTask2Handle) in the call to vTaskPrioritySet(). Listing 24 shows how
the handle was obtained. */
		PRINTF( "About to raise the Task2 priority\r\n" );
		vTaskPrioritySet( xTask2Handle, ( uxPriority + 1 ) );
		/* Task1 will only run when it has a priority higher than Task2.
Therefore, for this task to reach this point Task2 must already have
executed and set its priority back down to below the priority of this
task. */
	}
}

void vTask2Function( void *pvParameters )
{
	unsigned portBASE_TYPE uxPriority;
	/* Task1 will always run before this task as Task1 is created with the
higher priority. Neither Task1 nor Task2 ever block so will always be
in either the Running or the Ready state.
Query the priority at which this task is running - passing in NULL means
"return my priority". */
	uxPriority = uxTaskPriorityGet( NULL );
	for( ;; )
	{
		/* For this task to reach this point Task1 must have already run and
set the priority of this task higher than its own.
Print out the name of this task. */
		PRINTF( "Task2 is running\r\n" );
		/* Set our priority back down to its original value. Passing in NULL
as the task handle means "change my priority". Setting the
priority below that of Task1 will cause Task1 to immediately start
running again – pre-empting this task. */
		PRINTF( "About to lower the Task2 priority\r\n" );
		vTaskPrioritySet( NULL, ( uxPriority - 2 ) );
	}
}
