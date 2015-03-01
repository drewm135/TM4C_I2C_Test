/*
 * Author : Drew May
 * Date   : March 1st 2015
 * Version: 1.0
 * Summary: Tiva TM4C receives digit (1-9) from Raspberry Pi
 * 			connected through I2C and immediately transmits
 * 			digit back to Raspberry Pi. If '1' is received
 * 			then toggle a onboard LED.
 *
 * Pi Code: http://blog.oscarliang.net/raspberry-pi-arduino-connected-i2c/
 *
 * Note   : I have created corresponding C++ code if anyone would prefer.
 *          It is on the GitHub. Also the code used to toggle the LEDs on
 *          the Tiva are taken from the TivaWare drivers library and must
 *          be linked in to the project folder in a folder called drivers.
 */

/*
 * Pin Connections
 *
 * Ground is connected between both devices
 * PB2 on Tiva is connected to SCL (Pin 2) on the Raspberry Pi
 * PB3 on Tiva is connected to SDA (Pin 5) on the Raspberry Pi
 *
 * See http://pi.gadgetoid.com/pinout for a detailed
 * pinout of the Raspberry Pi. We are using Pins 2 and 5.
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/tm4c1294ncpdt.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/i2c.h"
#include "drivers/pinout.h"

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
// Configure the I2C0 and its pins.  This must be called before I2C0SlaveRX()
// and I2C0SlaveTX().
// Note: Changed function calls to ROM calls by adding MAP_ to statements.
//
//*****************************************************************************
void
ConfigureI2C0(void)
{
	//
	//enable I2C module 0
	//
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
	//
	//enable GPIO peripheral that contains I2C 0
	//
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	//
	// Configure the pin muxing for I2C0 functions on port B2 and B3.
	//
	MAP_GPIOPinConfigure(GPIO_PB2_I2C0SCL);
	MAP_GPIOPinConfigure(GPIO_PB3_I2C0SDA);
	//
	// Select the I2C function for these pins.
	//
	MAP_GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
	MAP_GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
	//
	// Enable the I2C0 slave module.
	//
	MAP_I2CSlaveEnable(I2C0_BASE);
	//
	// Set the slave address to SLAVE_ADDRESS.
	//
	MAP_I2CSlaveInit(I2C0_BASE, SLAVE_ADDRESS);
}

//*****************************************************************************
//
// Transmit received data back to I2C master. Toggle LED when 1 is received.
//
//*****************************************************************************
int
main(void)
{
    //
    // Run from the PLL at 120 MHz.
    //
    g_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
                SYSCTL_CFG_VCO_480), 120000000);

    uint32_t char_received = 0;

    uint8_t  led_on = 0;

    //
    // Configure the device pins.
    // Disables Ethernet and USB
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

    while(1)
    {

        //
        // Wait until the slave has received and acknowledged the data.
        //
        while(!(MAP_I2CSlaveStatus(I2C0_BASE) & I2C_SLAVE_ACT_RREQ));

        //
        // Read the data from the slave.
        //
        char_received = MAP_I2CSlaveDataGet(I2C0_BASE);

        if (char_received == 1) { //If a 1 is received we toggle the LED
        	if (led_on == 0) {
        		LEDWrite(CLP_D1, 1);
        		led_on = 1;
        	} else {
        		LEDWrite(CLP_D1, 0);
        		led_on = 0;
        	}
        }

        //
        // Wait until slave data is requested
        //
        while(!(MAP_I2CSlaveStatus(I2C0_BASE) & I2C_SLAVE_ACT_TREQ));

        MAP_I2CSlaveDataPut(I2C0_BASE, char_received); //Send back the data receieved earlier
    }
}
