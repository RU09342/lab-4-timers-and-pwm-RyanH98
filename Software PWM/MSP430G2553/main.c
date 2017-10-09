#include <msp430.h> 

#define PWM BIT2 //Initialize "PWM", "LED1", and "BUTTON" to their respective bits
#define LED1 BIT6
#define BUTTON BIT3
#define BUTTON_PRESSED (P1IN & BUTTON)

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer

    P1DIR |= PWM + LED1; //Enable LED and pin 1.2 as outputs.
    P1DIR &= ~BUTTON; //Enable button as input.
    P1OUT |= LED1; //Set the initial LED condition on.
    P1SEL &= ~PWM & ~LED1 & ~BUTTON; //Select I/O mode for pins 1.2, the LED, and the button.

    P1IE |= BUTTON; //Enable interrupts for button.
    P1REN |= BUTTON; //Enable the pull resistor on P1.3
    P1OUT |= BUTTON; //Tell the pull resistor to pull up
    P1IES |= BUTTON; //Set the button interrupt to trigger on a high->low edge.
    P1IFG &= ~BUTTON; //Clear the button interrupt flag register.

    TA0CCTL1 = OUTMOD_7; //Set capture/compare register 1 to set\reset mode. This will produce the PWM signal on pin 1.2
    TA0CCR0 = 1000 - 1; //Set the first compare register at 1kHz (this will set the period)
    TA0CCR1 = 500; //Set the second compare register at 500 for 50% duty cycle (this will set the duty cycle)
    TACTL = TASSEL_2 + MC_1; //Enable Clock A in 'up' mode.

    __enable_interrupt();           //Enable interrupts.

    __bis_SR_register(GIE);  //Enter low power mode with interrupts.

    while(1){
        if (BUTTON_PRESSED) //If the button is pressed, turn off LED1.
            P1OUT |= LED1;
        else //If the button isn't pressed, turn on LED1.
            P1OUT &= ~LED1;
    }
}

#pragma vector=PORT1_VECTOR //Set the port 1 interrupt routine
__interrupt void Port_1(void) {
    if (TA0CCR1 < 1000) //If TA0CCR1 is less than 1000, increment by 100.
        TA0CCR1 += 100;
    else //Else, set TA0CCR1 to 0.
        TA0CCR1 = 0;

    P1IFG &= ~BIT3; //Button IFG cleared.
}
