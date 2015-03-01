# TM4C_I2C_Test
Test to show I2C functionality between Ti Tiva TM4C microccontroller and Raspberry Pi

This project is a example of having a TM4C microcontroller act as a slave to a Raspberry Pi through I2C. A seperate Raspberry Pi program sends a number from 1-9 to the Tiva. The Tiva then relays that number back to the Pi. If the number is 1 then a LED on the Tiva development board is toggled.

Linking Instructions
----------------------------------
In CCS: Project->Build Options->ARM Compiler->Include Options.  Add your TivaWare directory
        Project->Build Options->ARM Linker  ->File Search Path. Add library file from TivaWare.
                Library File is in TivaWare\driverlib\ccs\debug\driverlib.lib
                
Also needed is pinout.c from the TivaWare TM4C1294XL drivers folder(In the examples section). Copy or link this file into a folder in your project called drivers.

Compile Instructions for C++ Program (If not using Python script mentioned below)
-----------------------------------
In order to run the C++ program the WiringPi library is needed. Instructions for installation are on the Wiring Pi website listed here: http://wiringpi.com/

For g++: I2cTest.cpp -o I2cTest -I/usr/local/include -L/usr/local/lib -lwiringPi
                
Contact me with any questions!!!
  drewm135@gmail.com

Examples influenced by:
  http://blog.oscarliang.net/raspberry-pi-arduino-connected-i2c/
  
#Note
There is a lot of junk here that I'm not going to take the time to ignore after its already been commited. Realistically nothing but .ccsproject, .cproject, .project, .settings folder and the source code are needed.
