#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P1DIR = BIT0; //red led

    P1DIR |= ~BIT6; //button


    P1OUT = 0x00; //start led off

     while(1){
         if (P1IN & BIT6){ //checks if button is not pressed
             P1OUT &= ~BIT0; //red led off
         }
         else{
             P1OUT = BIT0; //red led on
         }

     }
}
