#include <MKL25Z4.H>
int main(void) {
	SIM->SCGC5 |= 0x0400; /* enable clock to Port B */
	PORTB->PCR[18] = 0x100; /* make PTB18 pin as GPIO */
	PTB->PDDR |= 0x040000; /* make PTB18 as output pin */

	/* Configure SysTick */
	SysTick->LOAD = 8388000 - 1; /* reload with number of clocks per 200 ms */
	SysTick->CTRL = 5; /* enable it, no interrupt, use system clock */
	while (1) {
		if (SysTick->CTRL & 0x10000) /* if COUNT flag is set */
			PTB->PTOR = 0x040000; /* toggle red LED */
	}
}
