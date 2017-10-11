#include <msp430.h>

#define LED0 BIT0
#define LED1 BIT0
#define BUTTON BIT1
#define BUTTON_PRESSED (P1IN & BUTTON)

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer

    P2DIR |= LED1; //Enable LED1 as output.
    P1DIR |= LED0; //Enable LED0 as output.
    P2OUT &= ~LED1; //Set the initial LED1 condition on.
    P1OUT &= ~LED0; //Set initial LED0 condition off.

    P1DIR &= ~BUTTON; //Enable button as input.
    P1IE |= BUTTON; //Enable interrupts for button.
    P1REN |= BUTTON; //Enable the pull resistor on P1.1
    P1OUT |= BUTTON; //Tell the pull resistor to pull up
    P1IES |= BUTTON; //Set the button interrupt to trigger on a high->low edge.
    P1IFG &= ~BUTTON; //Clear the button interrupt flag register.

    TB0CTL = TBSSEL_2 + MC_1; //Select ACLK and enable Clock A in 'up' mode.

    TB0CCR0 = 1000; //Set the first compare register at 1kHz (this will set the period)
    TB0CCTL0 = CCIE; //Enable interrupts on capture/compare register 0.
    TB0CCTL0 &= ~TBIFG; //Clear CCR0 interrupt flag

    TB0CCR1 = 500; //Set the second compare register at half of CCR0 for 50% duty cycle (this will set the duty cycle)
    TB0CCTL1 = CCIE; //Enable interrupts on capture/compare register 1.
    TB0CCTL1 &= ~TBIFG; //Clear CCR1 interrupt flag

    PM5CTL0 &= ~LOCKLPM5;            //Disable high impedance mode.

    __enable_interrupt();           //Enable interrupts.

    __bis_SR_register(GIE);  //Enter interrupt mode.

    while(1){
        if (BUTTON_PRESSED)
            P2OUT &= ~LED1;
        else
            P2OUT |= LED1;
    }

}

#pragma vector=PORT1_VECTOR //Set the port 1 interrupt routine
__interrupt void Port_1(void) {
    if (TB0CCR1 < 1000) //If TB0CCR1 is less than TB0CCR0, increment it.
        TBCCR1 += 100;
    else //Else, set TB0CCR1 to 0.
        TBCCR1 = 0;

    P1IFG &= ~BUTTON; //Button IFG cleared.
}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer_B0(void) {
    P1OUT |= LED0;
}

#pragma vector=TIMER0_B1_VECTOR
__interrupt void Timer_B1(void) {
    switch (TB0IV){
        case 2: //CCR1 interrupt
            P1OUT &= ~LED0; //Turn LED0 off.
            break;
    }
}
