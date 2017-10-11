#include <msp430.h>

unsigned int time, timeButton = 0;
#define BUTTON BIT1                 //Define "BUTTON" as bit 3.
#define LED0 BIT0                   //Define "LED0" as bit 0.

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;       // stop watchdog timer

    P1DIR |= LED0;                  //Set P1.0 (LED) as an output
    P1OUT &= ~LED0;                 //Set the initial LED condition to off

    P1DIR &= ~BUTTON;               //Set P1.3 (Button) as an input
    P1REN |= BUTTON;                //Enable the pull resistor on P1.3
    P1OUT |= BUTTON;                //Tell the pull resistor to pull up
    P1IE |= BUTTON;                 //Enable interrupt on P1.3
    P1IES |= BUTTON;                //Set the P1.3 interrupt to trigger on a high->low edge.
    P1IFG &= ~BUTTON;               //Clear the interrupt flag register on P1.3

    PM5CTL0 &= ~LOCKLPM5;            //Disable high impedance mode.

    P1OUT &= ~LED0;                 //Set the initial LED condition to off.

    TA0CCTL0 = CCIE;                   //Enable interrupts for the first capture/compare register.
    TA0CTL = TASSEL_2 + MC_2; //Set the Clock_A control to:
                                    //1. TASSEL_2 which selects SMCLK, the internal 1MHz clock.
                                    //2. MC_2 which selects the continuous counting mode.
                                    //3. ID_2 which sets an internal 4x clock divider.

    __enable_interrupt();           //Enable interrupts.

    __bis_SR_register(LPM0 + GIE);  //Enter interrupt mode
}

//Interrupt vector service routine for Timer A0.
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void) {
    time++; //Increment time.
    if (time == timeButton + 3)
        P1IE |= BUTTON; //If 3 clock cycles have passed since timeButton was stored, re-enable button interrupts.
}

//Interrupt vector for button.
#pragma vector=PORT1_VECTOR         //Set the port 1 interrupt routine
__interrupt void Port_1(void) {
    P1OUT ^= LED0; //Toggle LED0.

    P1IE &= ~BUTTON;    //Disable button interrupt.
    P1IFG &= ~BUTTON;   //Clear button interrupt flag.
    timeButton = time;   //Store time in timeButton.
    P1IES ^= BUTTON;    //Switch the interrupt edge.
}

