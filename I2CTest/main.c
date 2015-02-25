/*
 * Author : Drew May
 * Data   : 2/25/2015
 * Version: 0.1
 * Summary: Tiva TM4C receives digit (1-9) from Raspberry Pi
 * 			connected through I2C and immediately transmits
 * 			digit back to Raspberry Pi. If '1' is received
 * 			then toggle a onboard LED.
 */

/*
 * Todo
 * Convert master transmitter method to slave receiver method
 * Convert main to match arduino sketch
 * Recomment main and receive methods
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "drivers/pinout.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/i2c.h"


//*****************************************************************************
//
// Set the address for slave module. This is a 7-bit address sent in the
// following format:
//                      [A6:A5:A4:A3:A2:A1:A0:RS]
//
// A zero in the "RS" position of the first byte means that the master
// transmits (sends) data to the selected slave, and a one in this position
// means that the master receives data from the slave.
//
//*****************************************************************************
#define SLAVE_ADDRESS 0x04

//*****************************************************************************
//
// System clock rate in Hz.
//
//*****************************************************************************
uint32_t g_ui32SysClock;

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

//*****************************************************************************
//
// Configure the I2C0 and its pins.  This must be called before I2C0MasterTX().
//
//*****************************************************************************
void
ConfigureI2C0(void)
{
	//
	//enable I2C module 0
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
	//
	//enable GPIO peripheral that contains I2C 0
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	//
	// Configure the pin muxing for I2C0 functions on port B2 and B3.
	//
	GPIOPinConfigure(GPIO_PB2_I2C0SCL);
	GPIOPinConfigure(GPIO_PB3_I2C0SDA);
	//
	// Select the I2C function for these pins.
	//
	GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
	GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
	//
	// Enable the I2C0 slave module.
	//
	I2CSlaveEnable(I2C0_BASE);
	//
	// Set the slave address to SLAVE_ADDRESS.
	//
	I2CSlaveInit(I2C0_BASE, SLAVE_ADDRESS);
}

//*****************************************************************************
//
// This must be called before I2C0MasterTX().
//
//*****************************************************************************4
//Todo Convert master transmitter to slave receiver method
void I2C0MasterTX(unsigned int address, unsigned char data)
{

	//
	// Specify slave address
	//
	I2CMasterSlaveAddrSet(I2C0_BASE, address, false);
	//
	// Place the character to be sent in the data register
	//
	I2CMasterDataPut(I2C0_BASE, data);
	//
	// Initiate send of character from Master to Slave
	//
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
	//
	// Wait for transmission to begin
	//
	while(!I2CMasterBusy(I2C0_BASE));
	//
	// Delay until transmission completes
	//
	while(I2CMasterBusy(I2C0_BASE));

}

//*****************************************************************************
//
// Transmit received data back to I2C master. Toggle LED when 1 is received.
//
//*****************************************************************************
//Todo Adjust method to match Arduino Sketch
int
main(void)
{
    //
    // Run from the PLL at 120 MHz.
    //
    g_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
                SYSCTL_CFG_VCO_480), 120000000);

    //
    // Configure the device pins.
    //
    PinoutSet(false, false);

    //
    // Enable the GPIO pins for the LED D1 (PN1).
    //
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);

    //
    // Initialize Master and Slave
    //
    ConfigureI2C0();

    //
    // We are finished.  Hang around flashing D1.
    //
    while(1)
    {
        //
        // Turn on D1.
        //
        LEDWrite(CLP_D1, 1);
        I2C0MasterTX(4, 72);

        //
        // Delay for a bit.
        //
        SysCtlDelay(g_ui32SysClock / 10 / 3);

        //
        // Turn off D1.
        //
        LEDWrite(CLP_D1, 0);
        I2C0MasterTX(4, 76);

        //
        // Delay for a bit.
        //
        SysCtlDelay(g_ui32SysClock / 10 / 3);
    }
}
