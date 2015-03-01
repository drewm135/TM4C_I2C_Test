// I2cTest
// Author: Drew May
// Based on http://blog.oscarliang.net/raspberry-pi-arduino-connected-i2c/
// Requires WiringPi http://wiringpi.com/
#include <iostream>
#include <wiringPi.h>
#include <wiringPiI2C.h>

const int address = 0x04; // This is the address we setup in the Tiva Program
int fd;

int writeNumber(unsigned value) {
	return wiringPiI2CWrite(fd, value);
}

int readNumber(void) {
	return wiringPiI2CRead(fd);
}

int main(int argc, const char * argv[]) {
	unsigned int var;
	int number;

	fd = wiringPiI2CSetup(address); // Automatically selects i2c on GPIO
	
	std::cin >> var;
	while(var) {
		writeNumber(var);
		std::cout << "Tiva, I sent you " << var << std::endl;
		number = readNumber();
		std::cout << "Tiva, I received a digit " << number << std::endl;
		std::cin >> var;
	}
	
	return 0;
}
