//----------------------------------------------------------------------------//
#include "touch_driver.h"
#include "i2c_driver.h"
#include <Arduino.h>
#include "lvgl.h"

//----------------------------------------------------------------------------//
#define GT_CMD_WR           0XBA      
#define GT_CMD_RD           0XBB        
#define GT911_MAX_WIDTH     320        
#define GT911_MAX_HEIGHT    480         
#define GT_CTRL_REG         0X8040       
#define GT_CFGS_REG         0X8047       
#define GT_CHECK_REG        0X80FF       
#define GT_PID_REG          0X8140      
#define GT_GSTID_REG        0X814E       
#define GT911_READ_XY_REG   0x814E             

//----------------------------------------------------------------------------//
GT911_Dev Dev_Now; 
GT911_Dev Dev_Backup;
bool touched = 0; 

//----------------------------------------------------------------------------//
uint8_t s_GT911_CfgParams[] =
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//----------------------------------------------------------------------------//
const uint8_t GT9111_CFG_TBL[] =
{
  0X60, 0X40, 0X01, 0XE0, 0X01, 0X05, 0X35, 0X00, 0X02, 0X08,
  0X1E, 0X08, 0X50, 0X3C, 0X0F, 0X05, 0X00, 0X00, 0XFF, 0X67,
  0X50, 0X00, 0X00, 0X18, 0X1A, 0X1E, 0X14, 0X89, 0X28, 0X0A,
  0X30, 0X2E, 0XBB, 0X0A, 0X03, 0X00, 0X00, 0X02, 0X33, 0X1D,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X32, 0X00, 0X00,
  0X2A, 0X1C, 0X5A, 0X94, 0XC5, 0X02, 0X07, 0X00, 0X00, 0X00,
  0XB5, 0X1F, 0X00, 0X90, 0X28, 0X00, 0X77, 0X32, 0X00, 0X62,
  0X3F, 0X00, 0X52, 0X50, 0X00, 0X52, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X0F,
  0X0F, 0X03, 0X06, 0X10, 0X42, 0XF8, 0X0F, 0X14, 0X00, 0X00,
  0X00, 0X00, 0X1A, 0X18, 0X16, 0X14, 0X12, 0X10, 0X0E, 0X0C,
  0X0A, 0X08, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0X00, 0X29, 0X28, 0X24, 0X22, 0X20, 0X1F, 0X1E, 0X1D,
  0X0E, 0X0C, 0X0A, 0X08, 0X06, 0X05, 0X04, 0X02, 0X00, 0XFF,
  0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
  0X00, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
  0XFF, 0XFF, 0XFF, 0XFF,
};

//----------------------------------------------------------------------------//
uint8_t GT911_WR_Reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t i;
    uint8_t ret = 0;
    IIC_Start();
    IIC_Send_Byte(GT_CMD_WR);     
    IIC_Wait_Ack();
    IIC_Send_Byte(reg >> 8);     
    IIC_Wait_Ack();
    IIC_Send_Byte(reg & 0XFF);    
    IIC_Wait_Ack();
    for (i = 0; i < len; i++)
    {
        IIC_Send_Byte(buf[i]);     
        ret = IIC_Wait_Ack();
        if(ret)  {  break;  }  
    }
    IIC_Stop();                   
    return ret;
}

//----------------------------------------------------------------------------//
void GT911_RD_Reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t i;
    IIC_Start();
    IIC_Send_Byte(GT_CMD_WR);   
    IIC_Wait_Ack();
    IIC_Send_Byte(reg >> 8);     
    IIC_Wait_Ack();
    IIC_Send_Byte(reg & 0XFF);     
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte(GT_CMD_RD);   
    IIC_Wait_Ack();
    for (i = 0; i < len; i++)
    {   buf[i] = IIC_Read_Byte(i == (len-1) ? 0 : 1); }
    IIC_Stop();
}

//----------------------------------------------------------------------------//
uint8_t GT911_Send_Cfg(uint8_t mode)
{
  uint8_t buf[2];
  uint8_t i = 0;
  buf[0] = 0;
  buf[1] = mode;  
  GT911_WR_Reg(GT_CHECK_REG, buf, 2);
  return 0;
}

//----------------------------------------------------------------------------//
void GT911_Scan(void)
{
    uint8_t buf[41];
    uint8_t Clearbuf = 0;
    uint8_t i;
    if (1)
    {
        Dev_Now.Touch = 0;
        GT911_RD_Reg(GT911_READ_XY_REG, buf, 1);

        if ((buf[0] & 0x80) == 0x00)
        {
            touched = 0;
            GT911_WR_Reg(GT911_READ_XY_REG, (uint8_t *)&Clearbuf, 1);
            delay(10);
        }
        else
        {
            touched = 1;
            Dev_Now.TouchpointFlag = buf[0];
            Dev_Now.TouchCount = buf[0] & 0x0f;
            if (Dev_Now.TouchCount > 5)
            {
                touched = 0;
                GT911_WR_Reg(GT911_READ_XY_REG, (uint8_t *)&Clearbuf, 1);
                //Serial.printf("Dev_Now.TouchCount > 5\r\n");
                return ;
            }
            GT911_RD_Reg(GT911_READ_XY_REG + 1, &buf[1], Dev_Now.TouchCount * 8);
            GT911_WR_Reg(GT911_READ_XY_REG, (uint8_t *)&Clearbuf, 1);

            Dev_Now.Touchkeytrackid[0] = buf[1];
            Dev_Now.X[0] = ((uint16_t)buf[3] << 8) + buf[2];
            Dev_Now.Y[0] = ((uint16_t)buf[5] << 8) + buf[4];
            Dev_Now.S[0] = ((uint16_t)buf[7] << 8) + buf[6];

            Dev_Now.Touchkeytrackid[1] = buf[9];
            Dev_Now.X[1] = ((uint16_t)buf[11] << 8) + buf[10];
            Dev_Now.Y[1] = ((uint16_t)buf[13] << 8) + buf[12];
            Dev_Now.S[1] = ((uint16_t)buf[15] << 8) + buf[14];

            Dev_Now.Touchkeytrackid[2] = buf[17];
            Dev_Now.X[2] = ((uint16_t)buf[19] << 8) + buf[18];
            Dev_Now.Y[2] = ((uint16_t)buf[21] << 8) + buf[20];
            Dev_Now.S[2] = ((uint16_t)buf[23] << 8) + buf[22];

            Dev_Now.Touchkeytrackid[3] = buf[25];
            Dev_Now.X[3] = ((uint16_t)buf[27] << 8) + buf[26];
            Dev_Now.Y[3] = ((uint16_t)buf[29] << 8) + buf[28];
            Dev_Now.S[3] = ((uint16_t)buf[31] << 8) + buf[30];

            Dev_Now.Touchkeytrackid[4] = buf[33];
            Dev_Now.X[4] = ((uint16_t)buf[35] << 8) + buf[34];
            Dev_Now.Y[4] = ((uint16_t)buf[37] << 8) + buf[36];
            Dev_Now.S[4] = ((uint16_t)buf[39] << 8) + buf[38];
            for (i = 0; i < Dev_Backup.TouchCount; i++)
            {    
                if (Dev_Now.Y[i] < 0)Dev_Now.Y[i] = 0;
                if (Dev_Now.Y[i] > 480)Dev_Now.Y[i] = 480;
                if (Dev_Now.X[i] < 0)Dev_Now.X[i] = 0;
                if (Dev_Now.X[i] > 320)Dev_Now.X[i] = 320;
            }
            for (i = 0; i < Dev_Now.TouchCount; i++)
            {
                if (Dev_Now.Y[i] < 0)touched = 0;
                if (Dev_Now.Y[i] > 480)touched = 0;
                if (Dev_Now.X[i] < 0)touched = 0;
                if (Dev_Now.X[i] > 320)touched = 0;

                if(touched == 1)
                {
                    Dev_Backup.X[i] = Dev_Now.X[i];
                    Dev_Backup.Y[i] = Dev_Now.Y[i];
                    Dev_Backup.TouchCount = Dev_Now.TouchCount;
                }
            }
            if(Dev_Now.TouchCount==0)
            {
                touched = 0;
            }  
        }
    }
}

//----------------------------------------------------------------------------//
uint8_t GT911_ReadStatue(void)
{
  uint8_t buf[4];
  GT911_RD_Reg(GT_PID_REG, (uint8_t *)&buf[0], 3); 
  GT911_RD_Reg(GT_CFGS_REG, (uint8_t *)&buf[3], 1);
  Serial.printf("TouchPad_ID:%d,%d,%d\r\nTouchPad_Config_Version:%2x\r\n", buf[0], buf[1], buf[2], buf[3]);
  return buf[3];
}

//----------------------------------------------------------------------------//
void Interrupt_callBack() 
{
  Serial.printf("ARDUINO_ISR_ATTR:\r\n");
}

//----------------------------------------------------------------------------//
void GT911_Reset_Sequence()
{
    I2C_RST (LOW);
    delay(100);
    I2C_RST (LOW);
    delay(100);
    I2C_RST (HIGH);
    delay(200);
}

//----------------------------------------------------------------------------//
void GT911_Int()
{
    uint8_t config_Checksum = 0, i;
    IIC_Init();
    GT911_Reset_Sequence();
    GT911_RD_Reg(GT_CFGS_REG, (uint8_t *)&s_GT911_CfgParams[0], 186);

    for (i = 0; i < sizeof(s_GT911_CfgParams) - 2; i++)
    {
        config_Checksum += s_GT911_CfgParams[i];
        Serial.printf("0x%02X  ", s_GT911_CfgParams[i]);
        if ((i + 1) % 10 == 0)
        {   Serial.printf("\r\n");  }
    }
    Serial.printf("0x%02X  0x%02X\r\nconfig_Checksum=0x%2X\r\n", s_GT911_CfgParams[184], s_GT911_CfgParams[185], ((~config_Checksum) + 1) & 0xff);

    if (s_GT911_CfgParams[184] == (((~config_Checksum) + 1) & 0xff))
    {
        Serial.printf("READ CONFIG SUCCESS!\r\n");
        Serial.printf("%d*%d\r\n", s_GT911_CfgParams[2] << 8 | s_GT911_CfgParams[1], s_GT911_CfgParams[4] << 8 | s_GT911_CfgParams[3]);

        if ((GT911_MAX_WIDTH != (s_GT911_CfgParams[2] << 8 | s_GT911_CfgParams[1])) || (GT911_MAX_HEIGHT != (s_GT911_CfgParams[4] << 8 | s_GT911_CfgParams[3])))
        {   
            s_GT911_CfgParams[1] = GT911_MAX_WIDTH & 0xff;
            s_GT911_CfgParams[2] = GT911_MAX_WIDTH >> 8;
            s_GT911_CfgParams[3] = GT911_MAX_HEIGHT & 0xff;
            s_GT911_CfgParams[4] = GT911_MAX_HEIGHT >> 8;
            s_GT911_CfgParams[185] = 1;

            Serial.printf("%d*%d\r\n", s_GT911_CfgParams[2] << 8 | s_GT911_CfgParams[1], s_GT911_CfgParams[4] << 8 | s_GT911_CfgParams[3]);

            config_Checksum = 0;
            for (i = 0; i < sizeof(s_GT911_CfgParams) - 2; i++)
            { config_Checksum += s_GT911_CfgParams[i];  }
            s_GT911_CfgParams[184] = (~config_Checksum) + 1;

            Serial.printf("config_Checksum=0x%2X\r\n", s_GT911_CfgParams[184]);
            Serial.printf("\r\n*************************\r\n");

            for (i = 0; i < sizeof(s_GT911_CfgParams); i++)
            {
                Serial.printf("0x%02X  ", s_GT911_CfgParams[i]);
                if ((i + 1) % 10 == 0)
                {    Serial.printf("\r\n"); }
            }
            Serial.printf("\r\n*************************\r\n");
            GT911_WR_Reg(GT_CFGS_REG, (uint8_t *)s_GT911_CfgParams, sizeof(s_GT911_CfgParams));
            GT911_RD_Reg(GT_CFGS_REG, (uint8_t *)&s_GT911_CfgParams[0], 186);

            config_Checksum = 0;
            for (i = 0; i < sizeof(s_GT911_CfgParams) - 2; i++)
            {
                config_Checksum += s_GT911_CfgParams[i];
                Serial.printf("0x%02X  ", s_GT911_CfgParams[i]);
                if ((i + 1) % 10 == 0)
                {   Serial.printf("\r\n"); }
            }
            Serial.printf("0x%02X  ", s_GT911_CfgParams[184]);
            Serial.printf("0x%02X  ", s_GT911_CfgParams[185]);
            Serial.printf("\r\n");
            Serial.printf("config_Checksum=0x%2X\r\n", ((~config_Checksum) + 1) & 0xff);
        }
    }
    GT911_ReadStatue();
}

//----------------------------------------------------------------------------//
void gt911_test(void)
{
    uint8_t i = 0;
}

//----------------------------------------------------------------------------//
uint8_t GT9111_Send_Cfg(uint8_t mode)
{
    uint8_t buf[2];
    uint8_t i = 0;
    buf[0] = 0;
    buf[1] = mode;
    GT911_WR_Reg(GT_CHECK_REG, buf, 2);
    return 0;
}

//----------------------------------------------------------------------------//
void gt911_int_(void) 
{
    uint8_t buf[4];
    uint8_t CFG_TBL[184];
  
    GPIO_I2C_Init();
    delay(50);

    I2C_RST (LOW);
    delay(10);
  
    I2C_RST (HIGH);
    delay(50);

    GT911_RD_Reg(0X8140, (uint8_t *)&buf, 4);
    Serial.printf("TouchPad_ID:%d,%d,%d\r\n", buf[0], buf[1], buf[2], buf[3]);
    buf[0] = 0x02;

    GT911_WR_Reg(GT_CTRL_REG, buf, 1);
    GT911_RD_Reg(GT_CFGS_REG, buf, 1);
    Serial.printf("Default Ver:0x%X\r\n", buf[0]);
    if (buf[0] < 0X60)
    {
        Serial.printf("Default Ver:0x%X\r\n", buf[0]);
        GT911_Send_Cfg(1);
    }
  
    GT911_RD_Reg(GT_CFGS_REG, (uint8_t *)&CFG_TBL[0], 184);
    for (uint8_t i = 0; i < sizeof(GT9111_CFG_TBL); i++)
    {
        Serial.printf("0x%02X  ", CFG_TBL[i]);
        if ((i + 1) % 10 == 0)
        {   Serial.printf("\r\n");  }
    } 
    delay(10);
    buf[0] = 0x00;
    GT911_WR_Reg(GT_CTRL_REG, buf, 1);
}

//----------------------------------------------------------------------------//
void  GT9147_Scan(uint8_t mode)
{
    uint8_t buf[41];
    GT911_RD_Reg(GT911_READ_XY_REG, buf, 1); 
    Serial.printf("GT911_READ_XY_REG:%d\r\n", buf[0]); 
}

//-------------------------------Read the touchpad-------------------------------//
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
    //uint16_t touchX, touchY;
    GT911_Scan();
    if ( touched == 0)
    { data->state = LV_INDEV_STATE_REL; }
    else
    {
        data->point.x = Dev_Now.X[0];  //Set the coordinates
        data->point.y = Dev_Now.Y[0];
       // Serial.printf("touch:%d, x_in:%d, y_in:%d, x_out:%d, y_out:%d\r\n", 
       // touched, Dev_Now.X[0], Dev_Now.Y[0], data->point.x, data->point.y);
        data->state = LV_INDEV_STATE_PR;
    }
}


