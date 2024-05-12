#include  <msp430g2553.h>
volatile unsigned int i = 0;            // Volatile to prevent optimization. This keeps count of cycles between LED toggles

void main(void) {
    WDTCTL = WDTPW + WDTHOLD;

    P1DIR |= BIT0;      // P1DIR is a register that configures the direction (DIR) of a port pin as an output or an input.

    P1DIR &= (~BIT3);     // Set P1.3 SEL as Input
    P1IE |= BIT3;         // Interrupt Enable
    P1IES |= BIT3;        // Trigger from High to Low and low to high
    P1IFG |= BIT3;        // Interrupt Flag


    while (1) {
        __low_power_mode_3();
    }
}

#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
    P1OUT ^= 0x01;
    P1IES ^= BIT3;
    P1IFG ^= BIT3;
}

