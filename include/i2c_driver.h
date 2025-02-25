#ifndef i2c_driver_h
#define i2c_driver_h

//----------------------------------------------------------------------------//
#include <Arduino.h>

//----------------------------------------------------------------------------//
void GPIO_I2C_Init(void);
void IIC_Init(void);
void I2C_RST (uint8_t comm);
void IIC_Start(void);
void IIC_Stop(void);
uint8_t IIC_Wait_Ack(void);
uint8_t IIC_Read_Byte(unsigned char );
void IIC_Send_Byte(uint8_t );

#endif 

