#include  <msp430g2553.h>
volatile unsigned int i = 0;            // Volatile to prevent optimization. This keeps count of cycles between LED toggles

char text[] = "Green\r\n";

void serial_output(char *str) {
    while (*str != 0) {

        while (!(IFG2 & UCB0TXIFG));
        UCB0TXBUF = *str++;

        while (!(IFG2&UCA0TXIFG));
        UCA0TXBUF = UCB0RXBUF;
    }
}


void initializeUART(){

    P1SEL |= BIT1 |BIT2;
    P1SEL2 |= BIT1|BIT2;

    UCA0CTL1 |= UCSWRST+UCSSEL_2;
    UCA0BR0 = 52;  //settings for 19200 baud
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS_0;
    UCA0CTL1 &= ~UCSWRST;
}

void spi_init() {

    P1SEL |= BIT5 + BIT6 + BIT7;
    P1SEL2 |= BIT5 + BIT6 + BIT7;

    UCB0CTL1 |= UCSWRST;

    // Configure USCB0 for SPI
    UCB0CTL0 |= UCCKPL + UCMST + UCSYNC + UCMSB; // Clock polarity high, master mode, synchronous mode, MSB first
    UCB0CTL1 |= UCSSEL_2;                        // Select SMCLK as the clock source

    // Set clock divider
    UCB0BR0 = BIT1;                              // SPI clock = SMCLK / 2
    UCB0BR1 = BIT0;                              // (High byte)

    // Initialize USCB0 by releasing it from reset
    UCB0CTL1 &= ~UCSWRST;
}

void main(void) {
    WDTCTL = WDTPW + WDTHOLD;

    P1DIR |= BIT0;      // P1DIR is a register that configures the direction (DIR) of a port pin as an output or an input.

    P1DIR &= (~BIT3);     // Set P1.3 SEL as Input
    P1IE |= BIT3;         // Interrupt Enable
    P1IES |= BIT3;        // Trigger from High to Low and low to high
    P1IFG |= BIT3;        // Interrupt Flag

    initializeUART();
    spi_init();

    serial_output(text);

    while (1) {
        __low_power_mode_3();
    }
}



#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{

    //P1IES ^= BIT3;    // If you want to set an interrupt when button press is released
    if(P1IFG & BIT3) {
        serial_output(text);
        P1OUT ^= 0x01;
        P1IFG &= ~BIT3; // Clear the Interrupt Flag
    }

}


