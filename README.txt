Dec 25, 2022

By: Jatin Jain 101184197

Here are my labs from a 3rd year real time systems course using the MSP432P401R SimpleLink™ Microcontroller LaunchPad™ (MSP‑EXP432P401R) to learn about real-time embedded systems 

Each lab description has been including below.

LAB 3 - Basic Input/Output (interfacing with LEDs and switches)
6 - Continuously poll the switches (with debouncing).
6.1 - When Switch 1 is pressed, the selected LED changes (default: RED LED).
6.2 - When Switch 2 is pressed, the current LED changes state.
- For the RED LED, there are only two states: on and off. This should be performed
through bitwise toggling. The RED LED should toggle approximately once a
second. The RED LED should be toggling independently from the switches and
the RGB LED behavior described below. DO NOT WORRY ABOUT PRECISE
TIMING OR THE RELATIVE SPEEDS OF USER AND APPLICATION FOR
NOW (press the Switches slowly and wait before you press again).
- For the RGB LED, there are 8 states, corresponding to off and 7 different colors.
This should be performed through bitwise clearing and setting. When you press
switch 1, the RGB LED will change color (it will increase from 0-7 every time
you press switch 1). When you press switch 2, the RGB LED will change color
(it will decrease from 7-0 every time you press switch 2). This allows us to cycle
through all the colors of the RGB LED, up and down.

BONUS 1
Use Switch 1 as a “counter”, and Switch 2 as “the enter key”. Count how many
times you press Switch 1 and change to that color when Switch 2 is pressed (0-7
are the only options; if someone presses more than 7 times, wrap-up to 0). This is
similar to a “sugar selection” in an automated coffee machine: every time if you
press the Switch 3 times, you want 3 sugars, and when you press “enter”, sugar is
dispensed. Here, the Switch will change colors to the desired one, based on how
many times you press the Switch

---------------------------

LAB 4 - BASIC IO WITH INTERRUPTS

7.1 - When Button 1 is pressed, the selected LED changes (default, RED
LED).
7.2 - When Button 2 is pressed, the current LED changes state.
For the RED LED, there are only two states: on and off. This should be performed
through bitwise toggling.
For the RGB LED, there are 8 states, corresponding to off and 7 different colors. This
should be performed through bitwise clearing and setting.
Remember the ISR must clear all required interrupt flags! Global variables should not be
(use “static” whenever is possible).
In other words, one button changes between the RED LED and the RGB LED: the other
button changes the current state of the current LED, allowing us to turn the RED LED on
and off and cycle through all the colors of the RGB LED

BONUS 1/2

Implement an interrupt-based counter. Whenever you press SW1, the counter is
incremented. Whenever you press SW2, you set a flag (an integer variable called
“Display”). Whenever you press SW1 and SW2 at the same time, you reset the counter.
There are two shared variables between the main program and the interrupt service
routine: the counter, and the Display flag. The main program is in an infinite loop,
checking the Display flag. If it is set (by the ISR), it should blink the red LED as many
times as the counter variable (with a short delay between each blink). Then, it should clear
the Display flag (and wait until you press SW2, which will trigger the ISR and will set
the Display flag again).



-------------------------------------------
LAB 5 - TIMERS

The main program should keep track of the current color of the traffic light, and
display the right color in the RGB LED.
2. Start with a simple example: have the Timer ISR configured so you can turn on
the red LED on for 0.5s, and then off for another 0.5s, periodically and forever.
The Timer ISR counts ticks. The main program uses the ticks counted, and decides
when the time to turn on/off the red LED is.
3. Modify the code above and program traffic light behavior. The traffic light goes
through an infinite cycle: green light for 5 seconds, yellow light for 1 second, red
light for 3 seconds. Implement a timer-based solution for this problem. The timer
ISR should keep track of the time, and when the right time comes, change the
traffic light state. This information is shared with the main program, who is in
charge of changing the colors as discussed in 1.
4. It is important to be careful in tracking the time of the ISR properly when the SW1
is pressed!

BONUS 1/2

If SW1 is pressed, it means a pedestrian wants to cross the street. The main program must
turn on the red LED when SW1 is pressed. The Timer ISR should record this event. If the
traffic light is green, 1 second after the button is pressed, the ISR should cycle to “yellow
light” state (yellow for 1 second, then red for 3 seconds, then start the normal cycle again).
SW1 is also interrupt-based.
Bonus Marks (2):
1. Put the main program in sleep mode to save energy after the light colors have
changed.
2. If the user presses SW2, this will simulate a power failure and reset. This will
make the RGB LED blink in red color with a period of 1s (1s red, 1s off, driven
by the timer ISR).

------------------------------

Lab 6 - BSP CONTROLLER
write a program that divides the range of joystick positions into 9 quadrants,


------------------
    |        |     
1   |  2     |  3
    |        |
------------------
    |        |
 4  |    0   |   5
    |        |
-----------------
    |        |
 6  |    7   |   8
    |        |
-----------------


1 – 880 Hz
2 – 988 Hz
3 – 1109 Hz
4 – 1175 Hz
5 – 1319 Hz
6 – 1480 Hz
7 – 1661 Hz
8 - 1760 Hz


BONUS 1

Convert the application into a Periodic Polling solution. Set up the Timer ISR to interrupt
every 20 ms. At that time, the joystick routine should be called, and the position should
be passed to the main program. The main program activates the buzzer with the desired
frequency value to make a sound according to the joystick position.
