#include <msp430g2553.h>

void setupSPI() {
    // Configure SPI
    UCB0CTL1 = UCSWRST;                     // Put eUSCI state machine in reset
    UCB0CTL0 = UCMST | UCSYNC | UCCKPL | UCMSB; // 3-pin, 8-bit SPI master, Clock polarity high, MSB first
    UCB0CTL1 |= UCSSEL_2;                   // Select SMCLK as clock source
    UCB0BR0 = 0x02;                         // /2 clock divider
    UCB0BR1 = 0;
    P1SEL |= BIT5 | BIT6 | BIT7;            // Set SPI pin functionality
    P2DIR |= BIT0;                          // Set P2.0 as output (SS)
    UCB0CTL1 &= ~UCSWRST;                   // Initialize eUSCI state machine
}

void sendSPI(unsigned char data) {
    P2OUT &= ~BIT0;                         // Pull SS low
    while (!(IFG2 & UCB0TXIFG));           // Wait for transmit buffer to be ready
    UCB0TXBUF = data;                       // Send data
    while (!(IFG2 & UCA0RXIFG));              // Wait for transmission to complete
    P2OUT |= BIT0;                          // Pull SS high
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    setupSPI();

    P1DIR |= BIT0;
    P1OUT |= BIT0;

    while (1) {
        sendSPI(0x01);                      // Send command to turn on LED
        __delay_cycles(500000);             // Delay
    }
}
