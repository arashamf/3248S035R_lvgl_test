#ifndef touch_driver_h
#define touch_driver_h

//----------------------------------------------------------------------------//
#include <Arduino.h>
#include "lvgl.h"

//----------------------------------------------------------------------------//
#define CT_MAX_TOUCH        5  

//----------------------------------------------------------------------------//
typedef struct
{
  uint8_t Touch;
  uint8_t TouchpointFlag;
  uint8_t TouchCount;

  uint8_t Touchkeytrackid[CT_MAX_TOUCH];
  uint16_t X[CT_MAX_TOUCH];
  uint16_t Y[CT_MAX_TOUCH];
  uint16_t S[CT_MAX_TOUCH];
} GT911_Dev;

//----------------------------------------------------------------------------//
void GT911_Scan(void);
void gt911_int_(void);
void my_touchpad_read( lv_indev_drv_t * , lv_indev_data_t * );

#endif 

