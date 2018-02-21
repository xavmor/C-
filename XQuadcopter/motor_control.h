/************************************************************
 * This header file will contain methods for motor handling.
 *
 * Motor 1 ---Pin 2.4--- FrontLeft Motor
 * Motor 2 ---Pin 2.5--- FrontRight Motor
 * Motor 3 ---Pin 2.6--- BackLeft Motor
 * Motor 4 ---Pin 2.7--- BackRight Motor
 *
 * The ESC, from what I have tested only accepts values from
 * 1,000 microseconds to 2,000 microseconds. Our PWM frequency
 * is 93.750 kHz. Each frequency pulse is about 10.66 uS.
 * This gives us 93 possible motor speeds, 1,000/10.66 = 93ish
 *
 * This timer TA0 is using the SMCLK clocking.
 *
 ***********************************************************/

/***********************************************************
 * Function: Controls the 4 motors and balances the drone.
 *
 * Passed: An array of halfwords. In this case 6 values. 3
 * 	values from the accelerometer and 3 from the gyroscope.
 *
 * Returned: Nothing
 *
 * Note: Keep in mind 93 is 0% motor power and 188 is 100%
 * 	motor power.
 *
 **********************************************************/
void motor_control(int16_t * data) {
/*********  Step 1 calculate the sensor data  *************/
;



/**********  Step 2 Send frequency to motors  **************/


}



/* This will initialize the PWM frequency to be 93.750 kHz
 * I am going to try to roll with it. This gives a pulse
 * that is equal to 10.666 MicroSeconds
 *
 * Pin Layout and Function:
 * 	TA0CCR1 = pin 2.4
 * 	TA0CCR2 = pin 2.5
 * 	TA0CCR3 = pin 2.6
 * 	TA0CCR4 = pin 2.7
 *
 *	These pins are initialized at 0% throttle
 *
 */
void init_PWM()
{
/******* Step 1 initialize the DCO Clock *****************/

	CS->KEY = 0x695A;           // This enable edits to any clock sources
	CS->CTL0 = 0;				// Delete all settings in this register
	CS->CTL0 |= 0x00800000;		// Enable DCO
	CS->CTL0 |= 0x00020000;		// Source DCO @ 6 MHz
	CS->CTL1 |=	0x00000070;		// DCOCLK selected for SMCLK sourceing
	CS->KEY = 0;				// Disable edits to clock


	/* Note:
	 *
	 *  At this point the SMCLK is at 6 MHz
	 *
	 ***/


/******** Step 2 initialize the PWM port using Timer_A**********/
	TA0CTL = 0;					// Delete the entire contents of timer
									// This will also stop the timer

	TA0R = 0;					// Start timer from 0

	TA0CTL |= 0x0200;			// Source from SMCLK (bits 8-9 = 10)
	TA0CTL |= 0x0080;			// Divide SMCLK by 4 (bits 6-7 = 11)
	TA0EX0 = 2;					// Divide the SMCLK by 3

	/* Note:
	 *
	 *  At this point the timer is running at 500 kHz
	 *		Basically intervals of 2 microseconds
	 ***/

/*********** Step 3 initialize the main comparing register ***********/
	TA0CCTL0 = 0x00E0;			// xxx ???
	TA0CCTL0 = 0x00D0;			// Toggle/set mode
	TA0CCR0 = 1250;				/* 2 microsecond pulses
									2 * 1250 = 400 refresh rate */


/*********** Step 4 initialize the other 4 comparing register ***********/
	TA0CCTL1 = 0x00E0;          // transition state needed
	TA0CCTL1 = 0x00D4;			//
	TA0CCR1 = 500;				// 0% of total power to motors

	TA0CCTL2 = 0x00E0;          // transition state needed
	TA0CCTL2 = 0x00D4;
	TA0CCR2 = 500;				// 0% of total power to motors

	TA0CCTL3 = 0x00E0;          // transition state needed
	TA0CCTL3 = 0x00D4;
	TA0CCR3 = 500;				// 0% of total power to motors

	TA0CCTL4 = 0x00E0;          // transition state needed
	TA0CCTL4 = 0x00D4;
	TA0CCR4 = 500;				// 0% of total power to motors


	// Final Step run timmer
	TA0CTL |= 0x0004;			// Reset the timer (implement all the changes)
	TA0CTL |= 0x0010;			// Up Mode Count/ Start timer

}



/********************************************************************
 *  Below function is only used for intializing the motors and setting
 *  	up the drone this should not be used for the actual drone
 *  	final product.
 *
 ********************************************************************/
void pwm_test() {
/******* Step 1 initialize the DCO Clock *****************/

	CS->KEY = 0x695A;           // This enable edits to any clock sources
	CS->CTL0 = 0;
	CS->CTL0 |= 0x00800000;
	CS->CTL0 |= 0x00020000;	// Source DCO @ 6 MHz
	CS->CTL1 |=	0x00000070;	// DCOCLK selected for SMCLK sourceing
	//CS->CTL1 |=	0x60000000;	// Divide the SMCLK by 64 (bits 28-30 = 011)
	CS->KEY = 0;				// Disable edits to clock

	/* Note:
	 *
	 *  At this point the SMCLK should be at 93.750 kHz
	 *
	 ***/


/******** Step 2 initialize the PWM port using Timer_A**********/
	/* We are going to set the pwm as close as possible *
	 * to 8 kHz but with the SMCLK at 93.750 kHz it is 	*
	 * going to be impossible. So we need to change it.	*
	 */

	TA0CTL = 0;					// Delete the entire contents of timer
									// This will also stop the timer

	TA0R = 0;					// Start timer from 0


	TA0CTL |= 0x0200;			// Source from SMCLK (bits 8-9 = 10)
	TA0CTL |= 0x0080;			// Divide SMCLK by 4 (bits 6-7 = 11)
	TA0EX0 = 2;					// Divide the SMCLK by 3


/*********** Step 3 initialize the main comparing register ***********/
	TA0CCTL0 = 0x00E0;			// xxx ???
	TA0CCTL0 = 0x00D0;			// Toggle/set mode
	TA0CCR0 = 1050;				// 128 microseconds pulse

	TA0CCTL1 = 0x00E0;			// xxx ???
	TA0CCTL1 = 0x00D4;			// Toggle/set mode
	TA0CCR1 = 500;

	P2DIR = 16;
	P2SEL0 = 16;
	P2SEL1 = 0;


	// Final Step run timmer
	TA0CTL |= 0x0004;			// Reset the timer (implement all the changes)
	TA0CTL |= 0x0010;			// Up Mode Count/ Start timer


/***********             Test Part                 **********************/
	// Side Buttons
	P1DIR &= ~0x12;
	P1REN |= 0x12;
	P1OUT |= 0x12;

	// Bottom Button
	P3DIR &= ~0x20;
	P3DIR |= 0x20;
	P3DIR |= 0x20;

	// Top Button
	P5DIR &= ~0x02;
	P5REN |= 0x02;
	P5OUT |= 0x02;

	while (1) {
		if (!(P1IN & 2))		// Lowest power
			TA0CCR1 = 500;
		else if (!(P1IN & 16))		// Highest power
			TA0CCR1 = 1000;
		else if (!(P5IN & 2)) {
			while (!(P5IN & 2))
				;
			if (TA0CCR1 <= 999)
				TA0CCR1++;
		}
		else if (!(P3IN & 32)) {
			while (!(P3IN & 32))
				;
			if (TA0CCR1 >= 501)
				TA0CCR1--;
		}
	}
}

/*
void stabelizer(int16_t xAccel, int16_t yAccel, int16_t xGyro, int16_t yGyro)
{
	// This if statement will take care of the x axis positioning
	if (xAccel > xxx || xAccel < xxx)
		;

	if(yAccel > xxx || YAccel < xxx)
		;

	if();
}
*/
