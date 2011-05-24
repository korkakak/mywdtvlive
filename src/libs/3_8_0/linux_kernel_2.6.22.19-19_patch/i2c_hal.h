#ifndef __I2C_HAL_H__
#define __I2C_HAL_H__

int I2C_Write(unsigned char SlaveAddress, unsigned char SubAddress, unsigned char *data, unsigned int n);
int I2C_Read(unsigned char SlaveAddress, unsigned char SubAddress, unsigned char *data, unsigned int n);
int I2C_Write_NoSubAddress(unsigned char SlaveAddress, unsigned char *data, unsigned int n);
int I2C_Read_NoSubAddress(unsigned char SlaveAddress, unsigned char *data, unsigned int n);
int I2C_Device_Detect(unsigned char SlaveAddress);
#endif
