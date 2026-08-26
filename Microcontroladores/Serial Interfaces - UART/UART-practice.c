/* UART functions */
void UART0_init(void);
void UART0_putc(char c);
char UART0_getc(void);
void UART0_puts(const char *str);

/* Menu functions */
void showMenu(void);
void processCommand(char option);

void optionLED(void);
void optionADC(void);
void optionKeypad(void);
void optionButtons(void);


/* -------------------------------------------------
 * UART FUNCTIONS
 * ------------------------------------------------- */

void UART0_init(void)
{
    /* Enable clock for UART0 */
    SIM->SCGC4 |= 0x0400;

    /* Use FLL output for UART baud rate generator */
    SIM->SOPT2 |= 0x04000000;

    /* Turn off UART0 while changing configurations */
    UART0->C2 = 0x00;

    /* 115200 Baud */
    UART0->BDH = 0x00;
    UART0->BDL = 0x0B;

    /* Over Sampling Ratio = 16 */
    UART0->C4 = 0x0F;

    /* 8-bit data, no parity */
    UART0->C1 = 0x00;

    /* Enable transmitter AND receiver */
    UART0->C2 = 0x0C;

    /* Enable clock for PORTA */
    SIM->SCGC5 |= 0x0200;

    /* PTA2 = UART0_TX */
    PORTA->PCR[2] = 0x0200;

    /* PTA1 = UART0_RX */
    PORTA->PCR[1] = 0x0200;
}

void UART0_putc(char c)
{
    /* Wait until transmit data register is empty */
    while (!(UART0->S1 & 0x80))
    {
    }

    UART0->D = c;
}


char UART0_getc(void)
{
    /* Wait until a character is received */
    while (!(UART0->S1 & 0x20))
    {
    }

    return UART0->D;
}


void UART0_puts(const char *str)
{
    while (*str != '\0')
    {
        UART0_putc(*str);
        str++;
    }
}


/* -------------------------------------------------
 * MENU
 * ------------------------------------------------- */

void showMenu(void)
{
    // TODO
}


/* -------------------------------------------------
 * MENU OPTIONS
 * ------------------------------------------------- */

void optionLED(void)
{
    // TODO
}


void optionADC(void)
{
    // TODO
}


void optionKeypad(void)
{
    // TODO
}


void optionButtons(void)
{
    // TODO
}


/* -------------------------------------------------
 * COMMAND PROCESSING
 * ------------------------------------------------- */

void processCommand(char option)
{
    // TODO
}


/* -------------------------------------------------
 * MAIN
 * ------------------------------------------------- */

int main(void)
{
    char option;

    /* Initialize peripherals */
    UART0_init();

    /* Show menu */
    showMenu();

    while (1)
    {
        option = UART0_getc();

        processCommand(option);
    }
}
