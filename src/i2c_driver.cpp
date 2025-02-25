//----------------------------------------------------------------------------//
#include "i2c_driver.h"
#include <Arduino.h>

//----------------------------------------------------------------------------//
//#define IIC_SCL_0  digitalWrite(IIC_SCL,LOW)
//#define IIC_SCL_1  digitalWrite(IIC_SCL,HIGH)
//#define IIC_SDA_0  digitalWrite(IIC_SDA,LOW)
//#define IIC_SDA_1  digitalWrite(IIC_SDA,HIGH)
#define READ_SDA   digitalRead(IIC_SDA)

//----------------------------------------------------------------------------//
int IIC_SCL = 32;
int IIC_SDA = 33;
int IIC_RST = 25;

//----------------------------------------------------------------------------//
void delay_us(unsigned int xus);
void SDA_IN(void);
void SDA_OUT(void);
void  I2C_SDA (uint8_t );
void  I2C_SCL (uint8_t );
uint8_t IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);

//----------------------------------------------------------------------------//
void delay_us(unsigned int xus)  //1us
{
  for (; xus > 1; xus--);
}

//----------------------------------------------------------------------------//
void SDA_IN(void)
{   
    pinMode(IIC_SDA, INPUT);    
}

//----------------------------------------------------------------------------//
void SDA_OUT(void)
{
  pinMode(IIC_SDA, OUTPUT);
}

//----------------------------------------------------------------------------//
void  I2C_SDA (uint8_t comm)
{
    digitalWrite(IIC_SDA, comm);
}

//----------------------------------------------------------------------------//
void  I2C_SCL (uint8_t comm)
{
    digitalWrite(IIC_SCL, comm);
}

//----------------------------------------------------------------------------//
void GPIO_I2C_Init(void)
{
  pinMode(IIC_SDA, OUTPUT);
  pinMode(IIC_SCL, OUTPUT);
  pinMode(IIC_RST, OUTPUT);
}

//----------------------------------------------------------------------------//
void IIC_Init(void)
{
    GPIO_I2C_Init();
    I2C_SDA (HIGH);
    I2C_SCL (HIGH);
}

//----------------------------------------------------------------------------//
void  I2C_RST (uint8_t comm)
{
    digitalWrite(IIC_RST, comm);
}

//----------------------------------------------------------------------------//
void IIC_Start(void)
{
    SDA_OUT();
    I2C_SDA (HIGH);
    I2C_SCL (HIGH);
    delay_us(4);
    I2C_SDA (LOW); //START:when CLK is high,DATA change form high to low
    delay_us(4);
    I2C_SCL (LOW);
}

//----------------------------------------------------------------------------//
void IIC_Stop(void)
{
    SDA_OUT();
    I2C_SCL (LOW);
    I2C_SDA (LOW); //STOP:when CLK is high DATA change form low to high
    delay_us(4);
    I2C_SCL (HIGH);
    I2C_SDA (HIGH);
    delay_us(4);
}

//----------------------------------------------------------------------------//
uint8_t IIC_Wait_Ack(void)
{
    uint8_t ucErrTime = 0;
    SDA_IN();      
    I2C_SDA (HIGH); 
    delay_us(1);
    I2C_SCL (HIGH);
    delay_us(1);
    while (READ_SDA)
    {
        ucErrTime++;
        if (ucErrTime > 250)
        {
            IIC_Stop();
            return 1;
        }
    }
    I2C_SCL (LOW);
    return 0;
}

//----------------------------------------------------------------------------//
void IIC_Ack(void)
{
  I2C_SCL (LOW);
  SDA_OUT();
  I2C_SDA (LOW);
  delay_us(2);
  I2C_SCL (HIGH);
  delay_us(2);
  I2C_SCL (LOW);
}

//----------------------------------------------------------------------------//
void IIC_NAck(void)
{
  I2C_SCL (LOW);
  SDA_OUT();
  I2C_SDA (HIGH); 
  delay_us(2);
  I2C_SCL (HIGH);
  delay_us(2);
  I2C_SCL (LOW);
}

//----------------------------------------------------------------------------//
void IIC_Send_Byte(uint8_t txd)
{
  uint8_t t;
  SDA_OUT();
  I2C_SCL (LOW);
  for (t = 0; t < 8; t++)
  {
    if ((txd & 0x80) >> 7)
    { I2C_SDA (HIGH);   }  
    else
    {  I2C_SDA (LOW); } 
    txd <<= 1;
    delay_us(2);  
    I2C_SCL (HIGH);
    delay_us(2);
    I2C_SCL (LOW);
    delay_us(2);
  }
}

//----------------------------------------------------------------------------//
uint8_t IIC_Read_Byte(unsigned char ack)
{
  unsigned char i, receive = 0;
  SDA_IN();
  for (i = 0; i < 8; i++ )
  {
    I2C_SCL (LOW);
    delay_us(2);
    I2C_SCL (HIGH);
    receive <<= 1;
    if (READ_SDA)receive++;
    delay_us(1);
  }
  if (!ack)
  { IIC_NAck(); }  
  else
  { IIC_Ack(); }  
  return receive;
}

//----------------------------------------------------------------------------//

