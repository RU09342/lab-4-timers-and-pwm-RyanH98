# Software PWM
### Ryan Hare

## Background
In order to control some types of electronic hardware (mainly electric motors), a method of signal control called Pulse Width Modulation is needed. Pulse width modulation or PWM is a method of sending a signal with a specific frequency to the device. The frequency remains the same, but the amount of time that the signal is high is changed. This is called the duty cycle. For example, if a signal has a 500Hz frequency and is high for half of the time, it has a 50% duty cycle. When a signal has the same frequency but is only high for the first 10% of a period, it has a 10% duty cycle. As an added bonus, changing the duty cycle of a signal going to an LED will change the percieved brightness of the LED.

To accomplish PWM through software, we can make use of timers. By setting a timer to 'Up' mode instead of 'Continuous' mode, we can change what value it counts up to by changing the value of CCR0. Likewise, we can create a second interrupt for the same timer by changing the value of CCR1. A PWM signal can be created by toggling an output signal on when CCR0 interrupts and off when CCR1 interrupts. By setting CCR0 to a constant (for example, 1000), we can create a PWM signal by changing the value of CCR1. The duty cycle of this signal will be controlled by CCR1. For example, when CCR0 is 1000 and CCR1 is 700, this would create a 70% duty cycle.

The code included does precisely that. It sets CCR0 and CCR1 values appropriately for a 50% duty cycle. Then, when the CCR0 interrupt triggers, it enables LED 0. When the CCR1 interrupt triggers, it disables LED 0. When the button is pressed, it increases the value of CCR1, increasing the duty cycle in 10% increments and, by extension, increases the brightness of the LED.

For this specific code, the LED is blinking at a rate of about 1kHz.
## Devices
Code is included for the MSP430G2553, MSP430FR5994, MSP430FR6989, MSP430FR2311, and MSP430F5529.
## Usage
The code can be programmed onto any of the compatible devices through the use of Code Composer Studio. Once programmed onto the device, the LED will turn on automatically and the button will change the brightness.
## Board-specific Code Differences
Aside from the pin differences between the boards, and the high impedance mode disable on the FRxxxx boards, the code is mostly the same between the different boards.

The main difference otherwise is that the MSP430FR2311 does not have a Timer A, so Timer B is used instead.
## Extra Work
For the extra work, I investigated how the power usage of the MSP430 changes based on the duty cycle of the PWM signal being sent to the LED. As shown in the graph below:

![UndividedPower](https://raw.githubusercontent.com/RU09342/lab-4-timers-and-pwm-RyanH98/master/Software%20PWM/Assets/NoID3Power.PNG)

The graph was taken as the duty cycle was varied from 0% to 100% through button presses. As shown, there is very much a correlation between the power usage of the MSP430 and the LED duty cycle. The power usage at 10% duty cycle was around 1mW. As the duty cycle was increased 10% at a time, it can be seen that the power usage steps up, increasing by about 1mW for every 10% the duty cycle was increased and maxing out at about 10mW at 100% duty cycle.

A similar trend can be seen on this graph of the energy usage:

![UndividedEnergy](https://raw.githubusercontent.com/RU09342/lab-4-timers-and-pwm-RyanH98/master/Software%20PWM/Assets/NoID3Energy.PNG)

As shown in the graph, the slope of the energy usage increases as the duty cycle increases. This means that the MSP430 is using more energy. This makes sense because although the frequency of the LED is remaining the same, the LED is being turned on for less time overall when the duty cycle is lower.

Additionally, the same measurements were taken when the clock used to control the PWM signal was divided by 8x:

![DividedPower](https://raw.githubusercontent.com/RU09342/lab-4-timers-and-pwm-RyanH98/master/Software%20PWM/Assets/ID3Power.PNG)

As shown in this graph, the toggling of the clock is much more apparent than in the graph that showed the undivided clock. The undivded power graph shows about the same power usage per duty cycle; around 2mW on 10% and around 10mW on 100%. However, the power usage also fluctuates quite heavily with the clock divided, making it seem as though it uses slightly less power overall. The graph of energy usage paints a similar picture:

![DividedEnergy](https://raw.githubusercontent.com/RU09342/lab-4-timers-and-pwm-RyanH98/master/Software%20PWM/Assets/ID3Energy.PNG)

The graph has a very similar slope to the non-clock-divided energy graph. Because there is so little variance in the slope, it is very hard to say if the undivided clock uses more energy than the divided clock.