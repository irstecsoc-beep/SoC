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
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void vIntegerGenerator(void *pvParameters);
static void vStringPrinter(void *pvParameters);
/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool pitIsrFlag = false;
TaskHandle_t * IntegerGenerator;
QueueHandle_t * xIntegerQueue;
QueueHandle_t * xStringQueue;
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
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(200000U, PIT_SOURCE_CLOCK));

    /* Enable timer interrupts for channel 0 */
    PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);

    /* Enable at the NVIC */
    EnableIRQ(PIT_IRQ_ID);

    xIntegerQueue = xQueueCreate( 10, sizeof( unsigned long ) );
    xStringQueue = xQueueCreate( 10, sizeof( char * ) );
    xTaskCreate( vIntegerGenerator, "IntGen", 1000, NULL, 1, IntegerGenerator );
    xTaskCreate( vStringPrinter, "String", 1000, NULL, 2, NULL );
    /* Start the scheduler so the created tasks start executing. */
    vTaskStartScheduler();
    for (;;)
    ;

}

static void vIntegerGenerator(void *pvParameters) {
	unsigned portLONG ulValueToSend = 0;
	int i;
	for (;;) {
		for (i = 0; i < 5; i++) {
			xQueueSendToBack(xIntegerQueue, &ulValueToSend, 0);
			ulValueToSend++;
		}
		/* Force an interrupt so the interrupt service routine can read the
		 values from the queue. */
		PRINTF("Generator task - About to generate an interrupt.\r\n");
		PIT_StartTimer(PIT, kPIT_Chnl_0);
		vTaskSuspend(IntegerGenerator);
		PRINTF("Generator task - Interrupt generated.\r\n\r\n\r\n");
	}
}

void PIT_LED_HANDLER(void) {
	static portBASE_TYPE xHigherPriorityTaskWoken;
	static unsigned long ulReceivedNumber;
	/* The strings are declared static const to ensure they are not allocated
	 on the stack of the ISR, and exist even when the ISR is not executing. */
	static const char *pcStrings[] = { "String 0\r\n", "String 1\r\n",
			"String 2\r\n", "String 3\r\n" };
	xHigherPriorityTaskWoken = pdFALSE;
	/* Loop until the queue is empty. */

	while (xQueueReceiveFromISR(xIntegerQueue, &ulReceivedNumber,
			&xHigherPriorityTaskWoken) != errQUEUE_EMPTY) {
		/* Truncate the received value to the last two bits (values 0 to 3 inc.), then send a pointer to the string that corresponds to the truncated value to a different queue. */
		ulReceivedNumber &= 0x03;
		xQueueSendToBackFromISR(xStringQueue, &pcStrings[ulReceivedNumber],
				&xHigherPriorityTaskWoken);
	}
	if (xHigherPriorityTaskWoken == pdTRUE) {
		PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
		pitIsrFlag = true;
		xTaskResumeFromISR(IntegerGenerator);
	}
}

static void vStringPrinter(void *pvParameters) {
	char *pcString;
	for (;;) {
		/* Block on the queue to wait for data to arrive. */
		xQueueReceive(xStringQueue, &pcString, portMAX_DELAY);
		/* Print out the string received. */
		PRINTF(pcString);
	}
}




