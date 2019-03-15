# RoboHacks2016
RoboHacks Arduino code for the GoonGuard

This is the code for the 'GoonGuard' to be run on an Arduino UNO, recieves a command input over serial (from a RaspberryPi 2 Model B in this case) and outputs it as robot movements. It also ran the blinky matrix, which gave the status of the robot at any given time. 

## esp.cpp
This is the C++ code to read the command pushed to the input USB port by the Raspberry Pi and pass it to the serial port for the arduino.

## _Goonguard folder
In this folder you will find 10 header files for the blinky matrix animations, and destroy.h, as well as the main Arduino code _goonguard.ino.

### Header files
This includes the blinky matrix animation header files. Each animation as a header file which declared a single constant declaration for each status image to be displayed on the 8x8 blinky matrix.

### Main Robot Code
This is the main function _goonguard.ino, which takes the command input over serial and parses it into 3 elements of movement: Direction, saved as dir, which hold the values "fwd" or "bwd", Action, saved as action, which holds "lft" and "rht", and a pwm value, saved as pwm, which holds values between 0 and 255. Each command is parsed and used to call the appropriate pins for movement direction & speed of each motor. Additionally it uses pointers and the animation object to dynamically allocate memory used by the blinky matrix. 


