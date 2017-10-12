#include <msp430.h> 

#define LED0 BIT0 //Define bits for the LEDs and BUTTON.
#define LED1 BIT1
#define BUTTON BIT3
#define BUTTON_PRESSED (BIN & BUTTON)

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer

    LEDDIR |= LED1; //Enable LED1 as output.
    BDIR &= ~BUTTON; //Enable button as input.
    LEDOUT |= LED1; //Set the initial LED condition on.

    BIE |= BUTTON; //Enable interrupts for button.
    BREN |= BUTTON; //Enable the pull resistor on P1.3
    BOUT |= BUTTON; //Tell the pull resistor to pull up
    BIES |= BUTTON; //Set the button interrupt to trigger on a high->low edge.
    BIFG &= ~BUTTON; //Clear the button interrupt flag register.

    TA0CCTL1 = OUTMOD7; //Set capture/compare register 1 to set\reset mode. This will produce the PWM signal on pin 1.2
    TA0CCR0 = 32; //Set the first compare register at 1kHz (this will set the period)
    TA0CCR1 = 16; //Set the second compare register at half of CCR0 for 50% duty cycle (this will set the duty cycle)
    TACTL = TASSEL_1 + MC_1; //Select ACLK and enable Clock A in 'up' mode.

    __enable_interrupt();           //Enable interrupts.

    __bis_SR_register(GIE);  //Enter low power mode with interrupts.

    while(1){
        if (BUTTON_PRESSED) //If the button is pressed, turn off LED1.
            LEDOUT |= LED1;
        else //If the button isn't pressed, turn on LED1.
            LEDOUT &= ~LED1;
    }
}

#pragma vector=PORT1_VECTOR //Set the port 1 interrupt routine
__interrupt void Port_1(void) {
    if (TA0CCR1 < 31) //If TA0CCR1 is less than 16000, increment by 1600.
        TA0CCR1 += 3;
    else //Else, set TA0CCR1 to 0.
        TA0CCR1 = 0;

    P1IFG &= ~BUTTON; //Button IFG cleared.
}
