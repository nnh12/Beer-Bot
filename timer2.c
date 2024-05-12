#include <msp430.h>

#define ONE_SEC 15500    // A macro to define a one second interrupt to increment the LED display

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;    // Stop WDT
    BCSCTL3 |= LFXT1S_2;         // ACLK = VLO

    // Configure Timer A0 to give us interrupts being driven by ACLK
    TA0CTL = TASSEL_1 + MC_2 + TAIE;  // ACLK, upmode, counter interrupt enable
    TA0CCR0 = ONE_SEC;  // Register 0 counter value to trigger interrupt
    TA0CCTL0 = CCIE;    // CCR0 interrupt enabled

    P1DIR |= BIT0;  // configure P1 as output

    __bis_SR_register(LPM3_bits + GIE);

    while(1)
    {
        P1OUT ^= BIT0;
        __low_power_mode_3(); // equivalent to __bis_SR_register(LPM3_bits);     // Enter LPM3

    }
}

// Timer 0 interrupt service routine for register 0 (count)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void)
{
    TA0CCR0 += ONE_SEC;                       // Increment register 0 value for next interrup
    __bic_SR_register_on_exit(LPM3_bits);     // Clear LPM3 bits from 0(SR)
}


