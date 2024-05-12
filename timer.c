#include <msp430.h>

#define TIMER_PERIOD 1000  // Timer period in milliseconds

volatile unsigned int timer_ticks = 0;
char text[] = "Green\r\n";

void initializeUART(){

    P1SEL = BIT1|BIT2;
    P1SEL2 = BIT1|BIT2;

    UCA0CTL1 |= UCSWRST+UCSSEL_2;
    UCA0BR0 = 52;  //settings for 19200 baud
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS_0;
    UCA0CTL1 &= ~UCSWRST;
}

void serial_output(char *str) {
    while (*str != 0) {
        while (!(IFG2&UCA0TXIFG));
        UCA0TXBUF = *str++;
    }
}


void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    TA0CCR0 = 500;             // Set timer period
    TA0CTL = TASSEL_2 + MC_1;       // SMCLK, Up mode
    TA0CCTL0 |= CCIE;               // Enable Timer_A interrupt

    P1DIR |= BIT0;
    initializeUART();

    __enable_interrupt();           // Enable global interrupts

}

// Timer_A interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    timer_ticks++;                  // Increment ticks

    if (timer_ticks >= TIMER_PERIOD)
    {
        serial_output(text);
        P1OUT ^= BIT0;              // Toggle LED
        timer_ticks = 0;            // Reset ticks
    }
}
