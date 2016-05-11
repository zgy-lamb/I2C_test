/*
	read from mpu6050
*/

#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include "i2c_control.h"
#include "mpu6050_i2c.h"

#define I2C_FILE_NAME "/dev/i2c-0"
#define mpu_Address 0x68

#define USAGE_MESSAGE \
    "Usage:\n" \
    "  %s r [addr] [register]   " \
        "to read value from [register]\n" \
    "  %s w [addr] [register] [value]   " \
        "to write a value [value] to register [register]\n" \
    ""
int i2c_file;
unsigned char tem[8];
struct MPUData_int MPUData_Raw ;
struct MPUData_float MPUData_Last ;

static int set_i2c_register(int file,
                            unsigned char addr,
                            unsigned char reg,
                            unsigned char value) {

    unsigned char outbuf[2];
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[1];

    messages[0].addr  = addr;
    messages[0].flags = 0;
    messages[0].len   = sizeof(outbuf);
    messages[0].buf   = outbuf;

    /* The first byte indicates which register we'll write */
    outbuf[0] = reg;

    /* 
     * The second byte indicates the value to write.  Note that for many
     * devices, we can write multiple, sequential registers at once by
     * simply making outbuf bigger.
     */
    outbuf[1] = value;

    /* Transfer the i2c packets to the kernel and verify it worked */
    packets.msgs  = messages;
    packets.nmsgs = 1;
    if(ioctl(file, I2C_RDWR, &packets) < 0) {
        perror("Unable to send data");
        return 1;
    }

    return 0;
}


static int get_i2c_register(int file,
                            unsigned char addr,
                            unsigned char reg,
                            unsigned char *val) {
    unsigned char inbuf, outbuf;
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];

    /*
     * In order to read a register, we first do a "dummy write" by writing
     * 0 bytes to the register we want to read from.  This is similar to
     * the packet in set_i2c_register, except it's 1 byte rather than 2.
     */
    outbuf = reg;
    messages[0].addr  = addr;
    messages[0].flags = 0;
    messages[0].len   = sizeof(outbuf);
    messages[0].buf   = &outbuf;

    /* The data will get returned in this structure */
    messages[1].addr  = addr;
    messages[1].flags = I2C_M_RD/* | I2C_M_NOSTART*/;
    messages[1].len   = sizeof(inbuf);
    messages[1].buf   = &inbuf;

    /* Send the request to the kernel and get the result back */
    packets.msgs      = messages;
    packets.nmsgs     = 2;
    if(ioctl(file, I2C_RDWR, &packets) < 0) {
        perror("Unable to send data");
        return 1;
    }
    *val = inbuf;

    return 0;
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

int main(int argc, char **argv) {
    

    // Open a connection to the I2C userspace control file.
    if ((i2c_file = open(I2C_FILE_NAME, O_RDWR)) < 0) {
        perror("Unable to open i2c control file");
        exit(1);
    }
    mpu6050_init();

    while(1){    
    readdata(); 
    //printf("%d, %d ,%d ,%d ,%d ,%d \n\r",MPUData_Raw.Gryo_X ,MPUData_Raw.Gryo_Y, MPUData_Raw.Gryo_Z, \
                                        MPUData_Raw.ACCEL_X, MPUData_Raw.ACCEL_Y ,MPUData_Raw.ACCEL_Z);
    printf("Gryo :%f, %f ,%f, ACCEL:%f ,%f ,%f \n\r",MPUData_Last.Gryo_X ,MPUData_Last.Gryo_Y, MPUData_Last.Gryo_Z, \
                                        MPUData_Last.ACCEL_X, MPUData_Last.ACCEL_Y ,MPUData_Last.ACCEL_Z);  
    sleep(1);
    }





// single read or write 
    if(argc > 3 && !strcmp(argv[1], "r")) {
        int addr = strtol(argv[2], NULL, 0);
        int reg = strtol(argv[3], NULL, 0);
        unsigned char value;
        if(get_i2c_register(i2c_file, addr, reg, &value)) {
            printf("Unable to get register!\n");
        }
        else {
            printf("Register %d: %d (%x)\n", reg, (int)value, (int)value);
        }
    }
    else if(argc > 4 && !strcmp(argv[1], "w")) {
        int addr = strtol(argv[2], NULL, 0);
        int reg = strtol(argv[3], NULL, 0);
        int value = strtol(argv[4], NULL, 0);
        if(set_i2c_register(i2c_file, addr, reg, value)) {
            printf("Unable to get register!\n");
        }
        else {
            printf("Set register %x: %d (%x)\n", reg, value, value);
        }
    }
    else {
        //fprintf(stderr, USAGE_MESSAGE, argv[0], argv[0]);
    }


    close(i2c_file);


    return 0;
}
