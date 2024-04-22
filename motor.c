#include "msp430g2553.h"


//notes for mary has a little lamb
int little_lamb[] = {0, 0, 0, E, D, C1, D, E, E, E, D, D, D, E, G, G, E, D, C1, D, E, E, E, E, D, D, E, D, C1, 0, 0, 0, 0, 0};
//duration for all the notes
int lamb_duration[] = {Q, Q, Q, Q, Q, Q, Q, Q, Q, H, Q, Q, H, Q, Q, H, Q, Q, Q, Q, Q, Q, H, Q, Q, Q, Q, H, Q, Q, Q, Q, Q, Q};
//gets the length of the song
int lamb_length = sizeof(little_lamb) / sizeof(little_lamb[0]);

//notes for twinkle twinkle little star
int twinkle_twinkle[] = {C1, C1, G, G, A, A, G, F, F, E, E, D, D, C1, G, G, F, F, E, E, D, G, G, F, F, E, E, D, C1, C1, G, G, A, A, G, F, F, E, E, D, D, C1, 0, 0, 0, 0, 0, 0, 0, 0};
//duration for all the notes
int twinkle_duration[] = {Q, Q, Q, Q, Q, Q, H, Q, Q, Q, Q, Q, Q, H, Q, Q, Q, Q, Q, Q, H, Q, Q, Q, Q, Q, Q, H, Q, Q, Q, Q, Q, Q, H, Q, Q, Q, Q, Q, Q, H, Q, Q, Q, Q, Q, Q, Q, Q};
//gets the length of the song
int twinkle_length = sizeof(twinkle_twinkle) / sizeof(twinkle_twinkle[0]);

//declares all global variables
int song;
int current_note;
int song_len;
int *note_pos;
int *duration_pos;

void  PlaySound(int *notes, int length, int *duration) { //sets a song to its beginning to be able to play
    song_len = length; //sets the length of the song
    note_pos = &notes[0]; //sets the notes position to the first note
    duration_pos = &duration[0]; //sets the duration of a note to the first element
    current_note = 0; //sets the current note to 0, ot the beginning of the song
}

void main(void){
  WDTCTL = WDTPW + WDTHOLD; // Stop WDT
  BCSCTL3 |= LFXT1S_2;      // ACLK = VLO

  BCSCTL1 = CALBC1_1MHZ;    // Set the DCO to 1 MHz
  DCOCTL = CALDCO_1MHZ;     // And load calibration data

  P2DIR |= BIT5; // We need P2.5 to be output
  P2DIR |= BIT1; // Set P2.1 as output
  P2SEL |= BIT5; // P2.5 is TA1.2 PWM output

  P2OUT &= ~BIT1;
  P2REN |= BIT1;

  //sets the interrupts for the button
  P1IE |= BIT3;
  P1IES |= BIT3;
  P1IFG &= ~BIT3;

  //P1DIR |= BIT6;
  P1DIR &= ~BIT3; //sets the button as an input
  //P1OUT |= BIT6;

  //sets up timer 0
  TA0CCTL0 = CCIE; //interrupt enable
  TA0CCR0 = 4000;
  TA0CTL = TASSEL_1 + MC_1 +ID_0;

  TA1CCTL2 = OUTMOD_7;
  TA1CTL = TASSEL_2 + MC_1; // SMCLK, upmode

  WDTCTL = WDT_ADLY_16;  // Set Watchdog Timer to 1000 ms with VLO
  IE1 |= WDTIE;

  //PlaySound(little_lamb, lamb_length, lamb_duration);
  __bis_SR_register(CPUOFF + GIE);
}

#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
    //switches which song is playing when the button is pressed
    song ^= 1;
    if (song) {
        PlaySound(little_lamb, lamb_length, lamb_duration); //initially plays mary has a little lamb
    }
    else {
        PlaySound(twinkle_twinkle, twinkle_length, twinkle_duration); //plays twinkle twinkle little star
    }
    P1IFG &= ~BIT3; //clears the interrupt flag
}
//deals with duration of notes
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void)
{
  if (current_note < song_len) {
      TA1CCR2 = 0; //deals with the spaces between notes
      TA1CCR0 = 0; //updates for a new note

      duration_pos = duration_pos + 1; //moves to the next duration
      note_pos = note_pos + 1; //moves to the next note
      current_note = current_note + 1;
  }
  else { //restart to beginning of song
      note_pos = note_pos - current_note;
      duration_pos = duration_pos - current_note;
      current_note = 0;
  }
}
//deals with the frequency
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void) {
    if (current_note < song_len) {
        TA1CCR2 = *note_pos >> 1; //divides by 2
        TA1CCR0 = *note_pos; //updates to new note
        TA0CCR0 = *duration_pos; //updates to new duration
    }
    else {
        //if song is over, reset values
        TA1CCR2 = 0;
        TA1CCR0 = 0;
        TA0CCR0 = 4000;
    }
}
