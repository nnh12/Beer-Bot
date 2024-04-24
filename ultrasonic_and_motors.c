/*
   ELEC327 Lab 5 Buzzer Example Code.
   Drive a piezo buzzer connected between P2.1 and P2.5.
   Plays a major C chord.
*/

//#include "msp430g2553.h"
/*
// Notes for a major C chord
int periods[] = {1000000/261.63,
   1000000/329.63,
   1000000/392.00,
   1000000/523.5};
int which_period = 0;
*/

#include <msp430.h>


/**
 * main.c
 */

void main(void){
  WDTCTL = WDTPW + WDTHOLD; // Stop WDT
  BCSCTL3 |= LFXT1S_2;      // ACLK = VLO

  BCSCTL1 = CALBC1_1MHZ;    // Set the DCO to 1 MHz
  DCOCTL = CALDCO_1MHZ;     // And load calibration data

  P2DIR |= BIT5; // We need P2.5 to be output
  P2DIR |= BIT1; // Set P2.1 as output
  P2SEL |= BIT5; // P2.5 is TA1.2 PWM output

  P1DIR |= BIT2; // We need P2.5 to be output
  //P1DIR |= BIT1; // Set P2.1 as output
  P1SEL |= BIT2 ; // P2.5 is TA1.2 PWM output

  //TA1CCR0 = periods[which_period];
  //TA1CCR2 = periods[which_period]>>1; // divde by 2
  //sets the interrupts for the button
  P1IE |= BIT3;
  P1IES |= BIT3;
  P1IFG &= ~BIT3;

  //P1DIR |= BIT6;
  P1DIR &= ~BIT3; //sets the button as an input
  //P1OUT |= BIT6;

  P1DIR = BIT0; //Green LED

  P1DIR |= ~BIT6; //input from the other board


  P1OUT = 0x00; //start led off

  TA1CCR0 = 7000;
  TA1CCR2 = 0;
  TA1CCTL2 = OUTMOD_7;
  TA1CTL = TASSEL_2 + MC_1; // SMCLK, upmode

  TA0CCR0 = 7000; //Set the period in the Timer A0 Capture/Compare 0 register to 1000 us.
  TA0CCTL1 = OUTMOD_7;
  TA0CCR1 = 0; //The period in microseconds that the power is ON. It's half the time, which translates to a 50% duty cycle.
  TA0CTL = TASSEL_2 + MC_1; //TASSEL_2 selects SMCLK as the clock source, and MC_1 tells it to count up to the value in TA0CCR0.

  WDTCTL = WDT_ADLY_250;  // Set Watchdog Timer to 1000 ms with VLO
  IE1 |= WDTIE;

  while(1){
      if (P1IN & BIT6){ //checks if button is not pressed
          P1OUT &= ~BIT0;
          TA1CCR2 = 5000;
          TA0CCR1 = 0;
      }
      else{
          P1OUT = BIT0; //red led on
          TA0CCR1 = 5000;
          TA1CCR2 = 0;
      }

  }


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

