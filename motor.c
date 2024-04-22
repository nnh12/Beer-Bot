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

  P2DIR |= BIT5 + BIT4; // We need P2.5 to be output
  P2DIR |= BIT1; // Set P2.1 as output
  P2SEL |= BIT5 + BIT4; // P2.5 is TA1.2 PWM output

  P1DIR |= BIT5;
  P1SEL |= BIT5;

  TA0CCR0 = 4000;
  TA0CCR2 = 3000;
  TA0CCTL2 = OUTMOD_7;
  TA0CTL = TASSEL_2 + MC_1; // SMCLK, upmode


  TA1CCR0 = 4000;
  TA1CCR2 = 3000;
  TA1CCTL2 = OUTMOD_7;
  TA1CTL = TASSEL_2 + MC_1; // SMCLK, upmode

  WDTCTL = WDT_ADLY_250;  // Set Watchdog Timer to 1000 ms with VLO
  IE1 |= WDTIE;
  __bis_SR_register(CPUOFF + GIE);
}

