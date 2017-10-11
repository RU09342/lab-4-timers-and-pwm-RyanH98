# Software Debouncing
### Ryan Hare

## Background
As buttons are mechanical devices, they have slight mechanical flaws that make them, at times, somewhat unsuitable for use in digital circuits. These flaws can be seen in any button interrupt circuit that doesn't contain some form of debouncing. When the button is depressed, the contact plate can bounce, triggering multiple button signals and sometimes, triggering the button interrupt more than once.

These 'false alarm' triggers on the button interrupt are not ideal when the code requires only one trigger per button press. Generally, the best way to solve this is through hardware. By adding a capacitor in series with the button, the signal can be discharged over time, skipping over the very rapid 'bounce' signals and ignoring them. However, a hardware solution is not always an option when you have constraints in your design.

In order to prevent these bounces through software, code to debounce the button is needed. The debouncing code uses a timer to create a small delay after a button interrupt where no more button interrupts are accepted. This essentially allows the main button input but prevents any 'bounce' inputs from triggering the interrupt in the code.
## Devices
There is code for the MSP430G2553, the MSP430FR6989, the MSP430FR5994, the MSP430FR2311, and the MSP430F5529.
## Usage
When the code is programmed onto the launchpad boards, the code will light the LED when the button is pressed down and turn the LED off when the button is depressed. This will show that the button interrupt is only triggering on the proper input signal edges and not on the bounces.
### Other Uses
This code is extremely useful in any situation where the button can not be debounced through hardware. A precise button signal is usually key to the successful and precise operation of a piece of code, so the debouncing is very important for ensuring this precision. Any code that uses a button and cannot debounce by hardware will benefit from debouncing.
## Board-Specific Code Differences
The code for the boards is mostly the same between them, however the pin assignments for the buttons changes by board.

* For the G2553, the button is on pin 1.3
* For the F5529, the button is on pin 2.1
* For the FR6989, the button is on pin 1.1
* For the FR5994, the button is on pin 5.6
* For the FR2311, the button is on pin 1.1

Additionally, for this code to actually work on the FR5994 and F5529, another line of code was needed.

PM5CTL0 &= ~LOCKLPM5;

This code disables high impedance mode, which sets all pins to high impedance inputs. This allows the board to actually read the input of the button and toggle the LED.