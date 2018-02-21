#include "msp.h"
#include "I2C_driver.h"
#include "motor_control.h"
#include "mpu6500.h"

/************************************************
 * Function Prototypes
 ***********************************************/
void print_values();











/**
 * main.c
 */
void main(void)

{
	pwm_test();

	while(1);

	MPU_TEST();

	while (1);

	/*
	 * Delete the function below when done or do something else with it later
	 */
	pwm_test(); 	// This will help calibrate the escs'

	int8_t DeviceAddress = 0x68;
	int8_t data[10];



/************************************************
 *     Step 1 - Initialize I2C protocol and
 *     configure the MPU6500
 ***********************************************/
	I2C_MasterMode_Setup();				// Initializes I2C on ports 1.6 & 1.7

	writeByte(DeviceAddress, SMPLRT_DIV_REGI, 0);		// Set sample rate at default (maybe 32 kHz)
	writeByte(DeviceAddress, CONFIG_REGI, FIFO_MODE);	// Activate FIFO_MODE
	writeByte(DeviceAddress, GYRO_CONFIG, GYRO_RANGE_2000);
	writeByte(DeviceAddress, ACCEL_CONFIG,0x18);		// Activate 16g accell scale
	writeByte(DeviceAddress, ACCEL_CONFIG_2, 0x08);
	writeByte(DeviceAddress, FIFO_EN, 0xF8);
	writeByte(DeviceAddress, INT_ENABLE, 1);
	//writeByte(DeviceAddress, USER_CTRL, xxx);

	while (1);

/************************************************
 *     Step 2 - Initialize the motors
 ***********************************************/

	init_PWM();							// PWM at 11,750 kHz

	//init_motors();





}








void print_test() {
	printf("Hello World");
}
