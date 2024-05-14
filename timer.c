#include <msp430.h>
#include <stdint.h>

#define TIMER_PERIOD 1000  // Timer period in milliseconds

volatile unsigned int timer_ticks = 0;
char text[] = "Green\r\n";

#define UART_BUFFER_SIZE 64

char uart_buffer[UART_BUFFER_SIZE];
volatile uint8_t uart_buffer_index = 0;

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


void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    TA0CCR0 = 500;             // Set timer period
    TA0CTL = TASSEL_2 + MC_1;       // SMCLK, Up mode
    TA0CCTL0 |= CCIE;               // Enable Timer_A interrupt

    P1DIR |= BIT0;
    initializeUART();

    __enable_interrupt();           // Enable global interrupts

}

// Timer_A interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    timer_ticks++;                  // Increment ticks

    if (timer_ticks >= TIMER_PERIOD)
    {
        serial_output(text);
        P1OUT ^= BIT0;              // Toggle LED
        timer_ticks = 0;            // Reset ticks
    }
}


#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    char received_char = UCA0RXBUF;  // Read received character
    if (received_char != '\r') {  // Check if received character is not carriage return
        uart_buffer[uart_buffer_index++] = received_char;  // Store character in buffer
        if (uart_buffer_index >= UART_BUFFER_SIZE) {  // Check if buffer overflow
            uart_buffer_index = 0;  // Reset buffer index
        }
    } else {
        uart_buffer[uart_buffer_index] = '\0';  // Null-terminate the received string
        // Handle received string here, for example, print it or process it further
        uart_buffer_index = 0;  // Reset buffer index for the next reception
    }
}
