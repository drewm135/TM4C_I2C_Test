# TM4C_I2C_Test
Simple test to show I2C functionality between Ti Tiva TM4C microccontroller and Raspberry Pi

This project is a simple example of having a TM4C microcontroller act as a slave to a Raspberry Pi through I2C. A seperate Raspberry Pi program sends a number from 1-9 to the Tiva. The Tiva then relays that number back to the Pi. If the number is 1 then a LED on the Tiva development board is toggled.

Examples influenced by:
  http://blog.oscarliang.net/raspberry-pi-arduino-connected-i2c/
