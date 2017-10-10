#include <msp430.h>

unsigned int timeStart, time, debouncing, button, lastButton = 0;
#define BUTTON BIT3                 //Define "BUTTON" as bit 3.
#define LED0 BIT0                   //Define "LED0" as bit 0.
#define INP (P1IN & BUTTON)         //Define "INP" for checking if there is an input on pin 1.3.

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;       // stop watchdog timer
    P1SEL &= (~LED0 & ~BUTTON);     //Select the I/O mode for P1.0 and P1.3

    P1DIR |= LED0;                  //Set P1.0 (LED) as an output
    P1OUT &= ~LED0;                 //Set the initial LED condition to off

    P1DIR &= ~BUTTON;               //Set P1.3 (Button) as an input
    P1REN |= BUTTON;                //Enable the pull resistor on P1.3
    P1OUT |= BUTTON;                //Tell the pull resistor to pull up
    P1IE |= BUTTON;                 //Enable interrupt on P1.3
    P1IES |= BUTTON;                //Set the P1.3 interrupt to trigger on a high->low edge.
    P1IFG &= ~BUTTON;               //Clear the interrupt flag register on P1.3

    P1OUT &= ~LED0;                 //Set the initial LED condition to off.

    CCTL0 = CCIE;                   //Enable interrupts for the first capture/compare register.
    TACTL = TASSEL_2 + MC_2; //Set the Clock_A control to:
                                    //1. TASSEL_2 which selects SMCLK, the internal 1MHz clock.
                                    //2. MC_2 which selects the continuous counting mode.
                                    //3. ID_2 which sets an internal 4x clock divider.

    __enable_interrupt();           //Enable interrupts.

    __bis_SR_register(GIE);         //Enter interrupt mode

    while (1)
    {
        if (debouncing != lastButton) {
            timeStart = time;}

        if ((time - timeStart) > 2) {
            button = debouncing;}

        if (button == 1) {
            P1OUT |= LED0;}
        else {
            P1OUT &= ~LED0;}

        lastButton = debouncing;
        debouncing = 0;
    }
}

//Interrupt vector service routine for Timer A0.
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void) {
    time = time + 1;

}

//Interrupt vector for button.
#pragma vector=PORT1_VECTOR         //Set the port 1 interrupt routine
__interrupt void Port_1(void) {
    debouncing = 1;                 //Enable the debouncing variable.
    P1IFG &= ~BUTTON;               //P1.3 IFG cleared
}

