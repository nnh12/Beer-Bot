
#include <msp430.h>

volatile int motor1 = 0;
volatile int motor2 = 0;
volatile int counter = 0;
volatile int state = 0;
volatile int temp;

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

  P1DIR = BIT0; //Green LED

  P1DIR |= ~BIT6; //input from the other board
  P2DIR |= ~BIT4; //input from the other board


  P1OUT = 0x00; //start led off

  //related to pin 1.2 FORWARD
  TA1CCR0 = 7000;
  TA1CCR2 = 0;
  TA1CCTL2 = OUTMOD_7;
  TA1CTL = TASSEL_2 + MC_1; // SMCLK, upmode

  //related to pin 2.5 BACKWARD
  TA0CCR0 = 7000; //Set the period in the Timer A0 Capture/Compare 0 register to 1000 us.
  TA0CCTL1 = OUTMOD_7;
  TA0CCR1 = 0; //The period in microseconds that the power is ON. It's half the time, which translates to a 50% duty cycle.
  TA0CTL = TASSEL_2 + MC_1; //TASSEL_2 selects SMCLK as the clock source, and MC_1 tells it to count up to the value in TA0CCR0.

  //WDTCTL = WDT_ADLY_250;  // Set Watchdog Timer to 1000 ms with VLO
  //IE1 |= WDTIE;

  P1IE |= BIT6;
  P1IES |= BIT6;
  P1IFG |= BIT6;

  P2IE |= BIT4;
  P2IES |= BIT4;
  P2IFG |= BIT4;

  P1OUT |= BIT0;


  __bis_SR_register(LPM1_bits + GIE);     // Enter LPM3
}


#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{

    if (P1IN & BIT6) // Check if pin 1.6 is high
    {
        // Toggle the values of TA1CCR2 and TA0CCR1
        if (TA1CCR2 == 5000) {
            TA1CCR2 = 0;
            TA0CCR1 = 5000;
            P1OUT ^= BIT0;
        }
        else if (TA0CCR1 == 5000) {
            TA1CCR2 = 0;
            TA0CCR1 = 0;
            }
    }

    //clear interrupt flag
    P1IES ^= BIT6;
    P1IFG &= ~BIT6;

}


#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void)
{
    state = counter % 2;
    if (!(P2IN & BIT4) & (state == 1)) {
        counter = counter + 1;
        TA0CCR1 = 5000;
        TA1CCR2 = 0;
        }
    else if (P2IN & BIT4) {
        counter = counter + 1;
        TA1CCR2 = 5000;
        P1OUT ^= BIT0;
    }


    P2IES ^= BIT4;
    P2IFG &= ~BIT4;
}
