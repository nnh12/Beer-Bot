#include <msp430.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

char text[] = "Green\r\n";

void setupSPI() {
    // Configure SPI
    UCB0CTL1 = UCSWRST;                     // Put eUSCI state machine in reset
    UCB0CTL0 = UCSYNC | UCCKPL | UCMSB;     // 3-pin, 8-bit SPI slave, Clock polarity high, MSB first
    P1SEL |= BIT5 | BIT6 | BIT7;            // Set SPI pin functionality
    P2DIR &= ~BIT0;                         // Set P2.0 as input (SS)
    P1DIR |= BIT1;                          // Set P1.1 as output (LED)
    P1OUT &= ~BIT1;                         // Ensure LED is off
    UCB0CTL1 &= ~UCSWRST;                   // Initialize eUSCI state machine
    UCB0I2CIE |= UCB0RXIE;                       // Enable USCI_B0 RX interrupt
}

void initializeUART(){

    P1SEL = BIT1|BIT2;
    P1SEL2 = BIT1|BIT2;

    UCA0CTL1 |= UCSWRST+UCSSEL_2;
    UCA0BR0 = 52;  //settings for 19200 baud
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS_0;
    UCA0CTL1 &= ~UCSWRST;
    UC0IE |= UCA0RXIE;  // Enable UART RX interrupt
}

void serial_output(char *str) {
    while (*str != 0) {
        while (!(IFG2&UCA0TXIFG));
        UCA0TXBUF = *str++;
    }
}


int main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    //setupSPI();
    initializeUART();
    serial_output(text);

    __bis_SR_register(GIE);                 // Enable global interrupts


    while (1) {
        char string[5];

        serial_output(text);

        // Main loop can be used for other tasks
        if (IFG2 & UCB0RXIFG) {
            unsigned char received = UCB0RXBUF; // Read received data
            ltoa(received, string, 10);
            serial_output(string);

            if (received == 0x01) {         // If received data is the command
                P1OUT ^= BIT1;              // Toggle the LED
            }
        }

    }
}
