#include <msp430g2553.h>  
int main(void) { 
    WDTCTL = WDTPW + WDTHOLD; //Disable the Watchdog timer for our convenience. 
    P1DIR |= BIT2; //Set pin 1.2 to the output direction. 
    P1SEL |= BIT2; //Select pin 1.2 as our PWM output.  
    TA0CCR0 = 1000; //Set the period in the Timer A0 Capture/Compare 0 register to 1000 us.  
    TA0CCTL1 = OUTMOD_7; 
    TA0CCR1 = 500; //The period in microseconds that the power is ON. It's half the time, which translates to a 50% duty cycle.
    TA0CTL = TASSEL_2 + MC_1; //TASSEL_2 selects SMCLK as the clock source, and MC_1 tells it to count up to the value in TA0CCR0. 
    __bis_SR_register(LPM0_bits); //Switch to low power mode 0. 
}
