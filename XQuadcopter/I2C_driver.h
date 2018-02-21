/*
 * I2C_driver.h
 *
 *  Created on: Dec 16, 2017
 *      Author: Xavier Cardenas
 */

#include "msp432p401r.h"

/********************************************************************************
 * Description: The function below sets up I2C communication on the msp432. The
 *	SMCLK clock source at 6 MHz is selected. It is then further divided to 400kHz.
 *	Multi-Master System mode not enabled.
 *
 *	Passed: Nothing
 *	Returned: Nothing
 *******************************************************************************/
void I2C_MasterMode_Setup() {

    // Lets initialize the SMCLK clock first
    CS->KEY = 0x695A;       // This is a password to enable clock edits
    CS->CTL1 &= 0x0FFFFFFF;     // Divide by 1, Bits 28-30 = 000b
    CS->CTL0 |= 0x00020000;     // Set DCO 6MHz, bits
    CS->KEY = 0x1111;           // Anything but the password locks the clock settings

    // Initialize Master Mode,  We are not setting multi-master system
    UCB0CTLW0 = 1;              // Halt State, Software reset enabled when bit 0 = 1
    UCB0CTLW0 |= 0x0600;        // Set the UCMODEx = 11
    UCB0CTLW0 |= 0x0100;        // Set UCSYNC = 1
    UCB0CTLW0 |= 0x0800;        // Set the UCMST = 1
    UCB0CTLW0 |= 0x80;          // Set the eUSCI_B clock source as SMCLK (6MHz)
    UCB0CTLW0 &= ~0xF000;       // Set bit UCSLA10 = 0 (7 bit address)

    // Basically 6,000,000/15 = 400,000 Hz
    UCB0BRW = 15;               // Set the frequency at 400 kHz

    // Implement all changes
    UCB0CTLW0 &= 0xFFFE;		// Release state

    // We need to initialize two pins for SDA and SCL communication
    P1SEL0 |= 0xC0;             // This needs to be set so that
    P1SEL1 &= 0x3F;             //			the I2C can be used on pins 1.6 & 1.7

    P1REN |= 0xC0;              // Enable Pull up resistors for
    P1OUT |= 0xC0;              // 							pins 1.6 and 1.7
}




/********************************************************************************
 * Description: This function will send a byte of data to a I2C device and write
 *		to that device.
 *
 * Passed: (device address, register address, value you want for specified register)
 * Returned: Nothing
 *******************************************************************************/
void writeByte(int8_t devAddress, int8_t regiAddress, int8_t value) {
/* Step 1 initiate the Master transmitter mode */

	UCB0CTLW0 |= 1;             // Keep state halted
	UCB0CTLW1 |= 8;				// Generate automatic stop depended on byte counter
	UCB0TBCNT = 2;				// Set the Byte COunter to 2 byte the reason is because i am sending register address and data
	UCB0CTLW0 &= ~0x0001;		// Release state

	UCB0I2CSA = devAddress;		// add address of device
	UCB0CTLW0 &= ~0x4000;		// Setting Slave as 7 bit address
	UCB0CTLW0 |= 0x0010;		// Set transmit mode (Set UCTR)
	UCB0CTLW0 |= 0x0002;		// Generate start condition (Set UCTXSTT);

/* Step 2, send data over to device */

	while(UCB0IFG & 2);			// Breaks when start condition is generated

	UCB0TXBUF = regiAddress;		// Write into the transmitter buffer

	while (UCB0CTLW0 & 2);		// Breaks when complete address has been sent

	while (!(UCB0IFG & 2));		// Breaks when data is being transmitted

	UCB0TXBUF = regiAddress;		// Write into the transmitter buffer

	while (UCB0CTLW0 & 2);		// Breaks when complete address has been sent

	while (UCB0CTLW0 & 4);		// Breaks when stop bit is set for sending
}





/********************************************************************************
 * Description: The function below does the following in order:
 * 	1. Send "Start bit" + "slave address" + "write bit"
 * 	2. Send register address that we want to read from the slave
 * 	3. Send Restart signal
 * 	4. Send "Slave address" + "read bit"
 * 	5. Read data into msp432 RXBUF register and generate stop bit
 *
 *	Passed: (slave's address, address of register, pointer to store info from slave)
 *	Returned: Nothing
 *******************************************************************************/
void readByte(int8_t devAddress, int16_t regAddress, int16_t * tdata) {
	/* Step 1 initiate the Master transmitter mode */

	UCB0CTLW0 |= 1;             // Keep state halted
	UCB0CTLW1 |= 4;				// Set byte counter with no automatic stop
	UCB0TBCNT = 1;				// Set the Byte COunter to 1 byte
	UCB0CTLW0 &= ~0x0001;				// Run halted state

	UCB0I2CSA = devAddress;		// add address of device
	UCB0CTLW0 &= ~0x4000;		// Setting Slave as 7 bit address
	UCB0CTLW0 |= 0x0010;		// Set transmit mode (Set UCTR)
	UCB0CTLW0 |= 0x0002;		// Generate start condition (Set UCTXSTT)

	while (UCB0STATW & 16);     // Bus is busy, Wait!

	while(UCB0IFG & 2);			// Wait until start condition is generated

	// We now can write into the transmitter buffer
	UCB0TXBUF = regAddress;

	while (UCB0CTLW0 & 2);		// Complete address not sent yet

	while (!(UCB0IFG & 2));		// Wait until data is transmitted


	// We can now send a restart condition
	UCB0CTLW0 |= 2;				// Generate a start/repeat signal, Set TXSTT bit
	UCB0CTLW0 &= ~0x0010;		// Set as receiver mode, clear UCTR bit


	while (UCB0CTLW0 & 2);		// Wait until complete address is sent

	UCB0CTLW0 |= 4;				// Generate a stop signal before we receive any bytes

	while (!(UCB0IFG & 1));		// Wait until we have received data into the buffer

	*tdata = UCB0RXBUF;

	while (UCB0CTLW0 & 4);		// Wait until the stop bit has been sent
}




/********************************************************************************
 * Description: The function below does the following in order:
 * 	1. Send "Start bit" + "slave address" + "write bit"
 * 	2. Send register address that we want to read from the slave
 * 	3. Send Restart signal
 * 	4. Send "Slave address" + "read bit"
 * 	5. Read data into msp432 RXBUF register multiple times
 * 	6. Generate a stop bit
 *
 *	Passed: (Device address, address of register, tdata to store info, number of Bytes)
 *	Returned: Nothing
 *******************************************************************************/
void readBytes(int8_t devAddress, int8_t regAddress, int8_t * tdata, int16_t numOfBytes) {
	/* Step 1 initiate the Master transmitter mode */

	UCB0CTLW0 |= 1;             // Keep state halted
	UCB0CTLW1 |= 4;				// Set automatic stop with byte counter
	UCB0TBCNT = numOfBytes;		// Set the Byte COunter to 4th passed variable above
	UCB0CTLW0 &= ~0x0001;		// Release state

	UCB0I2CSA = devAddress;		// add address of device
	UCB0CTLW0 &= ~0x4000;		// Setting Slave as 7 bit address
	UCB0CTLW0 |= 0x0010;		// Set transmit mode (Set UCTR)
	UCB0CTLW0 |= 0x0002;		// Generate start condition (Set UCTXSTT)

	/* Step 2 everything else */
	while(UCB0IFG & 2);			// Wait until start condition is generated

	// We now can write into the transmitter buffer
	UCB0TXBUF = regAddress;

	while (UCB0CTLW0 & 2);		// Complete address not sent yet

	while (!(UCB0IFG & 2));		// Wait until data is transmitted
}

