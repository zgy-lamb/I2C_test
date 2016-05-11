i2c_read_6050:i2c_control.o mpu6050_i2c.o core.o i2c_control.h mpu6050_i2c.h
	gcc i2c_control.o  mpu6050_i2c.o  core.o -o i2c_read_6050

i2c_control.o:i2c_control.c i2c_control.h
	gcc -c i2c_control.c -o i2c_control.o


mpu6050_i2c.o:mpu6050_i2c.c mpu6050_i2c.h i2c_control.h 
	gcc -c mpu6050_i2c.c -o mpu6050_i2c.o

core.o:core.c i2c_control.h mpu6050_i2c.h
	gcc -c core.c -o core.o
 
clean:
	rm -rf *.o i2c_read_6050