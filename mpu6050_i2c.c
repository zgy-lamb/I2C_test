#include "mpu6050_i2c.h"
#include "i2c_control.h"

void mpu_delay(int t){
	
}

int mpu6050_init(){
    int addr = MPU6050_DEVICE_ADDRESS;
    int reg;
    unsigned char value;

    if(get_i2c_register(i2c_file, addr, WHO_AM_I, &value)) {
        printf("Unable to get register! %x\n",WHO_AM_I);
    }
    else printf("Register 0x%x Who am I value: 0x%x \n", WHO_AM_I, (int)value );
	
    if(set_i2c_register(i2c_file, addr, PWR_MGMT_1, 0x00)) {
        printf("Unable to get register! %x\n",PWR_MGMT_1);	//return 0;
    }
    if(set_i2c_register(i2c_file, addr, SMPLRT_DIV, 0x07)) {
        printf("Unable to get register! %x\n",SMPLRT_DIV);
    }
    if(set_i2c_register(i2c_file, addr, CONFIG, 0x06)) {
        printf("Unable to get register! %x\n",CONFIG);
    }
    if(set_i2c_register(i2c_file, addr, GYRO_CONFIG, 0x08)) {
        printf("Unable to get register! %x\n",PWR_MGMT_1);
    }
    if(set_i2c_register(i2c_file, addr, ACCEL_CONFIG, 0x09)) {
        printf("Unable to get register! %x\n",PWR_MGMT_1);
    }

    return 1;
}

int readdata(){
    get_i2c_register(i2c_file, MPU6050_DEVICE_ADDRESS, ACCEL_XOUT_L, &tem[0]);
    get_i2c_register(i2c_file, MPU6050_DEVICE_ADDRESS, ACCEL_XOUT_H, &tem[1]);
    get_i2c_register(i2c_file, MPU6050_DEVICE_ADDRESS, ACCEL_YOUT_L, &tem[2]);
    get_i2c_register(i2c_file, MPU6050_DEVICE_ADDRESS, ACCEL_YOUT_H, &tem[3]);
    get_i2c_register(i2c_file, MPU6050_DEVICE_ADDRESS, ACCEL_ZOUT_L, &tem[4]);
    get_i2c_register(i2c_file, MPU6050_DEVICE_ADDRESS, ACCEL_ZOUT_H, &tem[5]);


    get_i2c_register(i2c_file, MPU6050_DEVICE_ADDRESS, GYRO_XOUT_L, &tem[6]);
    get_i2c_register(i2c_file, MPU6050_DEVICE_ADDRESS, GYRO_XOUT_H, &tem[7]);
    get_i2c_register(i2c_file, MPU6050_DEVICE_ADDRESS, GYRO_YOUT_L, &tem[8]);
    get_i2c_register(i2c_file, MPU6050_DEVICE_ADDRESS, GYRO_YOUT_H, &tem[9]);
    get_i2c_register(i2c_file, MPU6050_DEVICE_ADDRESS, GYRO_ZOUT_L, &tem[10]);
    get_i2c_register(i2c_file, MPU6050_DEVICE_ADDRESS, GYRO_ZOUT_H, &tem[11]);

    MPUData_Raw.ACCEL_X  = ( (int)tem[1] << 8 ) | tem[0] ;
    MPUData_Raw.ACCEL_Y  = ( (int)tem[3] << 8 ) | tem[2] ;
    MPUData_Raw.ACCEL_Z  = ( (int)tem[5] << 8 ) | tem[4] ;
    MPUData_Raw.Gryo_X = ( (int)tem[7] << 8 ) | tem[6] ;
    MPUData_Raw.Gryo_Y = ( (int)tem[9] << 8 ) | tem[8] ;
    MPUData_Raw.Gryo_Z = ( (int)tem[11] << 8 ) | tem[10] ;
   
    MPUData_Last.Gryo_X = (float)MPUData_Raw.Gryo_X / 16.4;
    MPUData_Last.Gryo_Y = (float)MPUData_Raw.Gryo_Y / 16.4;   
    MPUData_Last.Gryo_Z = (float)MPUData_Raw.Gryo_Z / 16.4;
    MPUData_Last.ACCEL_X = (float)MPUData_Raw.ACCEL_X / 8192 * 9.8; 
    MPUData_Last.ACCEL_Y = (float)MPUData_Raw.ACCEL_Y / 8192 * 9.8; 
    MPUData_Last.ACCEL_Z = (float)MPUData_Raw.ACCEL_Z / 8192 * 9.8; 
}
