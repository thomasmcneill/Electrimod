# Electrimod
PIC18 C Libarary

For C18 compiler.  Would need to be updated for the new MPLAB-XC compiler

This was intended for a modular stackable set of boards similar to arduino but after a failed Kickstarter I abandoned  it.

*A2D Libary

*ANSI markup library for Serial terminal

*Boot loader using Xmodem to upload new code

*Command Prompt Libary, yep it works.  Lets you hook commands with parameters to functions.  You can use it to view current variables, set values, run functions.  It rocks.  With support for backspace, delete, and arrow keys

*CRC Libary

*DS1307 Library (Not sure if it is complete)

*Interrupts Library, it manages your interrupts for you to make life easier

*MAX518 DAC Library 

*PWM Library

*Serial IO Library

*Servo Libary

*I2C Library

*Timers library

------------------------

main.c (PICF2525)

This is an example program that was for running an a lawnmower using propane with electronic ignition using an electronic spark plug and controlled injector.  Awesome right?  The application shows how to use Timer Queues, Servos, Pin interrupts, Storing configuration in EEPROM, ANSI display with colors and other cool stuff, reading input from serial port.
