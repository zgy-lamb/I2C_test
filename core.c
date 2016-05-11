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

#define USAGE_MESSAGE \
    "Usage:\n" \
    "  %s r [addr] [register]   " \
        "to read value from [register]\n" \
    "  %s w [addr] [register] [value]   " \
        "to write a value [value] to register [register]\n" \
    ""


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
