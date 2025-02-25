//----------------------------------------------------------------------------//
#include <Arduino.h>
#include "graphics_init.h"
#include "graphics.h"
#include "touch_driver.h"
#include "WiFi_init.h"
#include "lvgl.h"
//#include <demos/lv_demos.h>  

//----------------------------------------------------------------------------//
uint8_t count;

//---------------------------------------------------------------------------------------//
void IRAM_ATTR onTimer()
{
  lv_tick_inc(1);  
}

//---------------------------------------------------------------------------------------//
void timerinit()
{
  static hw_timer_t *timer1 = NULL;
  timer1 = timerBegin(1, 240, true); //240-предделитель таймера
  timerAttachInterrupt(timer1, onTimer, true);
  timerAlarmWrite(timer1, 1000, true); //1000 - счётчик таймера
  timerAlarmEnable(timer1);
}


//---------------------------------------------------------------------------------------//
void setup()
{
  Serial.begin(115200);

  gt911_int_();
  lvgl_init ();
  timerinit();
  input_driver_init ();

  screens_init ();
  init_WiFi_connection ();
  
}

//---------------------------------------------------------------------------------------//
void loop() 
{
  WiFi_connection_check ();
  lv_task_handler();
  delay (5);
}

