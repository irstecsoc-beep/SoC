#include <MKL25Z4.H>

int main(void) {

	SIM->SCGC5 |= 0x1000; /* enable clock to Port D */

	PORTD->PCR[1] = 0x400; /* set PTD1 pin for TPM0CH1 */

	SIM->SCGC6 |= 0x01000000; /* enable clock to TPM0 */

	SIM->SOPT2 |= 0x01000000;

	/* use MCGFLLCLK as timer counter clock */
	TPM0->SC = 0; /* disable timer while configuring */
	TPM0->SC = 0x07; /* prescaler /128 */
	TPM0->MOD = 0xFFFF; /* max modulo value */
	TPM0->CONTROLS[1].CnSC = 0x14; /* OC toggle mode */
	TPM0->CONTROLS[1].CnSC |= 0x80; /* clear CHF */
	TPM0->CONTROLS[1].CnV = TPM0->CNT + 32766; /* schedule next transition */
	TPM0->SC |= 0x08; /* enable timer */

	while (1) {
		/* wait until the CHF is set */
		while (!(TPM0->CONTROLS[1].CnSC & 0x80)) {
		}
		TPM0->CONTROLS[1].CnSC |= 0x80; /* clear CHF */
		TPM0->CONTROLS[1].CnV = TPM0->CNT + 32766;
		/* schedule next transition */
	}
}
