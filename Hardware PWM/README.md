# Hardware PWM
### Ryan Hare

## Background
The software method of creating a PWM signal (which can be found [here](https://github.com/RU09342/lab-4-timers-and-pwm-RyanH98/tree/master/Software%20PWM)) is a very good method of creating a PWM signal. However, the MSP430 boards have an even easier method of creating a PWM signal.

The timer on the MSP430 boards can be used to directly output a PWM value to a pre-assigned pin, provided the correct pin is assigned as an output. To do this, the method is very similar to the software PWM in that the CCR0 and CCR1 registers need to be initialized. CCR0 will define what the timer counts to and therefor, the period, while CCR1 will be a smaller value that defines the duty cycle of the PWM signal. The only difference between the hardware and software PWM is that in the software, the timer interrupts are enabled on CCR0 and CCR1. In the hardware, the following is used instead:

TA0CCTL1 = OUTMOD7;

CCR1 is set to output mode 7. Output mode 7 is predefined as set/reset mode. What this means is that when the timer hits the value in CCR1, it will set the PWM pin high. When the timer hits the value of CCR0, it will reset the PWM pin to be low. This makes creating a PWM signal much easier than the software method.

However, this ease comes at a price. Since the pin that the PWM signal is assigned to is pre-defined in the board, an external LED will need to be used in order to actually use the PWM signal. This means that a BJT or MOSFET will need to be put in place to limit the current out of the PWM pin and make sure that the processor is not damaged. A circuit for doing so can be found in the offboard blink lab, located [here](https://github.com/RU09342/lab-2-blinking-leds-RyanH98/tree/master/Off_Board%20Blink).
## Devices
Code is included for the MSP430G2553, the MSP430FR2311, the MSP430FR6989, the MSP430FR5994, and the MSP430F5529.
## Usage
Once the board has been programmed using Code Composer Studio, the LED circuit will need to be wired to the correct pin. A list of these pins can be found below in the Board-specific differences section. Once the pin is wired up and the LED is powered, the board should provide a PWM signal to the LED. Pressing the button will increase the duty cycle by 10%, and it will loop back to 0% if it hits 100%.
## Board-specific Code Differences
The boards share similar code. Aside from the button pin assignments, the main difference between the boards is which pin they use for the PWM signal.
* The G2553 uses pin 1.2 for the PWM signal.
* The FR2311 uses pin 2.0 for the PWM signal.
* The FR5994 uses pin 1.1 for the PWM signal.
* The FR6989 uses pin 1.0 for the PWM signal.
* The F5529 uses pin 4.5 for the PWM signal.
## Extra Work
For the extra work on this part, I tried making the PWM work with the ACLK instead of the SMCLK. The SMCLK on all the boards runs at several megahertz. In contrast, the ACLK, which runs at 32768kHz, is extremely slow in comparison.

Despite the far slower speed of the ACLK, it is still possible to make a PWM signal run at 1kHz. With the SMCLK, the CCR0 value needed to get a kHz PWM signal was very high. In contrast, for the ACLK, the value needed for CCR0 to get a 1kHz signal is very small, only about 32. Likewise, the CCR1 value for a 50% duty cycle is only 16, and it increments by 3 to increase the duty cycle by 10%.

Code for the FR2311 that uses ACLK instead of SMCLK can be found in the extra work folder. ([Here](https://github.com/RU09342/lab-4-timers-and-pwm-RyanH98/tree/master/Hardware%20PWM/Extra%20Work))
