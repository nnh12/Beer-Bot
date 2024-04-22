/*
   ELEC327 Lab 5 Buzzer Example Code.
   Drive a piezo buzzer connected between P2.1 and P2.5.
   Plays a major C chord.
*/

#include "msp430g2553.h"
/*
// Notes for a major C chord
int periods[] = {1000000/261.63,
   1000000/329.63,
   1000000/392.00,
   1000000/523.5};
int which_period = 0;
*/

void main(void){
  WDTCTL = WDTPW + WDTHOLD; // Stop WDT
  BCSCTL3 |= LFXT1S_2;      // ACLK = VLO

  BCSCTL1 = CALBC1_1MHZ;    // Set the DCO to 1 MHz
  DCOCTL = CALDCO_1MHZ;     // And load calibration data

  P2DIR |= BIT5; // We need P2.5 to be output
  P2DIR |= BIT1; // Set P2.1 as output
  P2SEL |= BIT5; // P2.5 is TA1.2 PWM output

  P1DIR |= BIT5; // We need P2.5 to be output
  //P1DIR |= BIT1; // Set P2.1 as output
  P1SEL |= BIT5 ; // P2.5 is TA1.2 PWM output





  TA1CCR0 = 7000;
  TA1CCR2 = 5000;

  TA1CCTL2 = OUTMOD_7;
  TA1CCTL1 = OUTMOD_7;
  TA1CTL = TASSEL_2 + MC_1; // SMCLK, upmode

  WDTCTL = WDT_ADLY_250;  // Set Watchdog Timer to 1000 ms with VLO
  IE1 |= WDTIE;
  __bis_SR_register(CPUOFF + GIE);
}
/*
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
{
  which_period = (which_period + 1) % 4;
  TA1CCR2 = periods[which_period]>>1;
  TA1CCR0 = periods[which_period];
}
*/
/*
#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
    if (P1IN & BIT3) //when the button is released
    {

    }

}

    }
*/
