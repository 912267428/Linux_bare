#ifndef _BSP_I2C_H
#define _BSP_I2C_H

#include "imx6ul.h"


#define I2C_STATUS_OK				(0)
#define I2C_STATUS_BUSY				(1)
#define I2C_STATUS_IDLE				(2)
#define I2C_STATUS_NAK				(3)
#define I2C_STATUS_ARBITRATIONLOST	(4)
#define I2C_STATUS_TIMEOUT			(5)
#define I2C_STATUS_ADDRNAK			(6)

enum i2c_direction
{
    kI2C_Write = 0x0,   
    kI2C_Read = 0x1,
};

struct i2c_transfer
{
    unsigned char slaveAddress; //从机地址
    enum i2c_direction direction;
    unsigned int subaddress;    //寄存器地址
    unsigned char subaddressSize;    // 寄存器地址长度 	
    unsigned char *volatile data;   //数据缓冲区
    volatile unsigned int dataSize;		
};

void i2c_init(I2C_Type *base);
unsigned char i2c_master_start(I2C_Type *base, unsigned char address, enum i2c_direction direction);
unsigned char i2c_master_repeated_start(I2C_Type *base, unsigned char address,  enum i2c_direction direction);
unsigned char i2c_check_and_clear_error(I2C_Type *base, unsigned int status);
unsigned char i2c_master_stop(I2C_Type *base);
void i2c_master_write(I2C_Type *base, const unsigned char *buf, unsigned int size);
void i2c_master_read(I2C_Type *base, unsigned char *buf, unsigned int size);
unsigned char i2c_master_transfer(I2C_Type *base, struct i2c_transfer *xfer);


#endif