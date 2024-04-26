#include <msp430.h>

void main(void){
  WDTCTL = WDTPW + WDTHOLD; // Stop WDT
  BCSCTL3 |= LFXT1S_2;      // ACLK = VLO

  BCSCTL1 = CALBC1_1MHZ;    // Set the DCO to 1 MHz
  DCOCTL = CALDCO_1MHZ;     // And load calibration data

  //setting up inputs and outputs:

  P2DIR |= BIT5; // We need P2.5 to be output
  P2DIR |= BIT1; // Set P2.1 as output
  P2SEL |= BIT5; // P2.5 is TA1.2 PWM output

  P1DIR |= BIT2; // We need P2.5 to be output
  P1SEL |= BIT2 ; // P2.5 is TA1.2 PWM output

  P1DIR = BIT0; //Green LED

  P1DIR |= ~BIT6; //input from the other board for the ultrasonic sensors
  P2DIR |= ~BIT3; //input from the other board for the weight sensor


  P1OUT = 0x00; //start led off

  //related to pin 1.2 which controls FORWARD
  TA1CCR0 = 7000; //setting up the duty cylce of the motors
  TA1CCR2 = 0;    //setting to 0 will have the motors off
  TA1CCTL2 = OUTMOD_7;   // want to use outmode 7
  TA1CTL = TASSEL_2 + MC_1; // SMCLK, upmode

  //related to pin 2.5 which controls BACKWARD
  TA0CCR0 = 7000; //Set the period in the Timer A0 Capture/Compare 0 register to 1000 us.
  TA0CCTL1 = OUTMOD_7; // want to use outmode 7
  TA0CCR1 = 0; //setting to 0 will have the motors off
  TA0CTL = TASSEL_2 + MC_1; //TASSEL_2 selects SMCLK as the clock source, and MC_1 tells it to count up to the value in TA0CCR0.

  //setting up the interrupts for pin 1.6 which corresponds to the ultrasonic sensors
  P1IE |= BIT6;  //interrupt enable
  P1IES |= BIT6; //interrupt edge select, will activate on low to high
  P1IFG |= BIT6; //interrupt flag

  //setting up the interrupts for pin 2.3 which corresponds to the weight sensor
  P2IE |= BIT3;  //interrupt enable
  P2IES |= BIT3; //interrupt edge select, will activate on low to high
  P2IFG |= BIT3; //interrupt flag

  __bis_SR_register(LPM1_bits + GIE);     // Enter LPM1
}

//interrupt service used for ultrasonic sensors
#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
    //when either of the ultrasonic sensors detects a long distance (like the edge of a table)
    if (P1IN & BIT6) // Check if pin 1.6 is high
    {
        // If the car is moving forward, will change to moving backwards
        if (TA1CCR2 == 5000) {
            TA1CCR2 = 0;     //turns pwm controlling fowards off
            TA0CCR1 = 5000;  //turns pwm controlling backwards on
        }
        //If the car is going backwards, will stop the car
        else if (TA0CCR1 == 5000) {
            TA1CCR2 = 0; //turns pwm controlling fowards off
            TA0CCR1 = 0; //turns pwm controlling backwards off
            }
    }

    //clear interrupt flag
    P1IES ^= BIT6;
    P1IFG &= ~BIT6;

}

//interrupt service for the weight sensor
#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void)
{
    //when something is placed on the weight sensor
    if (P2IN & BIT3) {            //check if pin 2.3 is high
        TA1CCR2 = 5000;     //turns pwm controlling fowards on
        TA0CCR1 = 0;        //ensures the pwm controlling backwards is off
    }

    //clear interrupt flag
    P2IES ^= BIT3;
    P2IFG &= ~BIT3;
}
