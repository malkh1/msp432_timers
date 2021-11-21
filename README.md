# msp432_timers

This project makes use of buttons, LEDs, TimerA, and Timer32 to implement a program that sets two timers; 
- a timer that goes off every 4 minutes: TimerA
- a timer that goes off every hour: Timer32

Button 1 starts/resets the 1-hour timer.
Button 2 starts/resets the 4-minute timer.

When the timers go off, a red LED is turned on. Each timer gets its own red LED.

I made this program so it reminds me to drink water every 4 minutes, and to notify me when an hour has passed so that I can take a break from my pc.
