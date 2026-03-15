#include "MKL25Z4.h"

int main(void) {

	void delayMs(int n);

	PORTD->PCR[0] = 0x100; /* make PTD0 pin as GPIO */
	PORTD->PCR[1] = 0x100; /* make PTD1 pin as GPIO */
	PORTD->PCR[2] = 0x100; /* make PTD2 pin as GPIO */
	PORTD->PCR[3] = 0x100; /* make PTD3 pin as GPIO */
	PORTD->PCR[7] = 0x103;
	/* make PTD7 pin as GPIO and PE */

	PTD->PDDR |= 0x0F; /* make PTD0-3 as output pin */
	PTD->PDDR &= ~0x80; /* make PTD7 as input pin */

	if ((PTD->PDIR & 0x80) == 0) /* PTD7 == 0 */
	{
		PTD->PDOR &= ~0x0F; /* open all switches */
		delayMs(100); /* wait 0.1 second */
		PTD->PDOR |= 0x09; /* close SW1 & SW4 */
		while ((PTD->PDIR & 0x80) == 0)
			; /*PTD7 == 0 */
	}

	else /* PTD7 == 1 */
	{
		PTD->PDOR &= ~0x0F; /* open all switches */
		delayMs(100); /* wait 0.1 second */
		PTD->PDOR |= 0x06; /* close SW2 & SW3 */
		while ((PTD->PDIR & 0x80) != 0)
			; /*PTD7 == 0 */
	}
}

void delayMs(int n) {
	int i;
	int j;
	for (i = 0; i < n; i++)
		for (j = 0; j < 7000; j++) {
		}
}
