//Program (5): Reading the value from a digital input
#include <MKL25Z4.H>

int main (void) {

	SIM->SCGC5 |= 0x400; /* enable clock to Port B */

	PORTB->PCR[19] = 0x100; /* make PTB19 pin as GPIO */

	PTB->PDDR |= 0x80000; /* make PTB19 as output pin */

	SIM->SCGC5 |= 0x200; /* enable clock to Port A */

	PORTA->PCR[1] = 0x103; /* make PTA1GPIO & en pull-up */

	PTA->PDDR &= ~0x02; /* make PTA1 as input pin */

	while (1) {

		/* check to see if switch is pressed */
		if (PTA->PDIR & 2)
			/* if not, turn off green LED */
			PTB->PSOR = 0x80000;
		else
			PTB->PCOR = 0x80000; /* turn on green LED */
	}
}
