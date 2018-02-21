/************************************************************
 * This is a driver for the MPU6500 Gyroscope/Accelerometer
 *
 * Note: For more details on specifics with the MPU6500. Refer
 * 		to the data sheet and the register map.
 *
 ************************************************************/




/*********    Internal MPU6500 address registers    *********/

#define SMPLRT_DIV_REGI		0x19	// Sample Rate Divider
#define CONFIG_REGI			0x1A	// Configuration register
#define GYRO_CONFIG			0x1B	// Name implies definition
#define ACCEL_CONFIG		0x1C	// Name implies definition
#define ACCEL_CONFIG_2		0x1D	// Name implies definition
#define FIFO_EN				0x23	// FIFO Buffer settings
#define INT_ENABLE			0x38	// Enables interrupt(s)
#define USER_CTRL			0x6A	// More settings



/**** MPU6500 register names within the address register ****/
#define FIFO_MODE			0x40	// Used for Register 26
#define	EXT_SYNC_TEMP		0x08	// Used for Register 26
#define	EXT_SYNC_GYRO_XOUT	0x10	// FSYNC bit location
#define	EXT_SYNC_GYRO_YOUT	0x18	// FSYNC bit location
#define	EXT_SYNC_GYRO_ZOUT	0x20	// FSYNC bit location
#define	EXT_SYNC_ACCEL_XOUT	0x28	// FSYNC bit location
#define	EXT_SYNC_ACCEL_YOUT	0x30	// FSYNC bit location
#define	EXT_SYNC_ACCEL_ZOUT	0x38	// FSYNC bit location
#define DLPF_CFG			0x07	//
//#define GYRO_RANGE_250		0x	//
#define GYRO_RANGE_500		0x08	//
#define GYRO_RANGE_1000		0x10	//
#define GYRO_RANGE_2000		0x18	//
#define FCHOICE_B_32kHz		0x11	//
//#define FCHOICE_B_32kHz		0x01
//#define FCHOICE_B_32kHz		0x10
/*#define
#define
#define
#define
#define
#define
*/




void MPU_TEST() {
	I2C_MasterMode_Setup();				// Initializes I2C on ports 1.6 & 1.7

	int *data;

	readByte(0x68, SMPLRT_DIV_REGI, data);		// Set sample rate at default (maybe 32 kHz)
	printf("%d", data);
	readByte(0x68, CONFIG_REGI, data);	// Activate FIFO_MODE
	printf(data);
	readByte(0x68, GYRO_CONFIG, data);
	printf(data);
	readByte(0x68, ACCEL_CONFIG, data);		// Activate 16g accell scale
	printf(data);
	readByte(0x68, ACCEL_CONFIG_2, data);
	printf(data);
	readByte(0x68, FIFO_EN, data);
	printf(data);
	readByte(0x68, INT_ENABLE, data);


	while (1);
}



