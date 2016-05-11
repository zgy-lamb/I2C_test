#ifndef _mpu6050_H
#define _mpu6050_H

#include "i2c_control.h"

#define	SMPLRT_DIV		0x19	
#define	CONFIG			0x1A	
#define	GYRO_CONFIG		0x1B
#define	ACCEL_CONFIG	0x1C
#define	ACCEL_CONFIG2 0x1D

#define INT_PIN_CFG     0x37
#define INT_ENABLE      0x38
#define INT_STATUS      0x3A    


#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C

#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E

#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40

#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	

#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46

#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

#define MPU_USER_CTRL_REG 	0x6A
#define MPU_FIFO_CNTL_REG		0x73
#define	PWR_MGMT_1		0x6B	
#define	WHO_AM_I			0x75	

#define MPU6050_DEVICE_ADDRESS 0x68
#define MPU6050_ID 0x68

struct MPUData_int{
    short Gryo_X;
    short Gryo_Y;
    short Gryo_Z;
    short ACCEL_X;
    short ACCEL_Y;
    short ACCEL_Z;
};
struct MPUData_float{
    float Gryo_X;
    float Gryo_Y;
    float Gryo_Z;
    float ACCEL_X;
    float ACCEL_Y;
    float ACCEL_Z;
};

void mpu_delay(int t);
int mpu6050_init();
int readdata();
unsigned char tem[8];

struct MPUData_int MPUData_Raw ;
struct MPUData_float MPUData_Last ;

#endif
