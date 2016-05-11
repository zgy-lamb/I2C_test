#ifndef _i2c_control_H
#define _i2c_control_H

#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

int i2c_file;
int set_i2c_register(int file,
                            unsigned char addr,
                            unsigned char reg,
                            unsigned char value);
int get_i2c_register(int file,
                            unsigned char addr,
                            unsigned char reg,
                            unsigned char *val);



#endif