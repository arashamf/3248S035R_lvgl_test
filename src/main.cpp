//-----------------------------------------------------------------------------------------//
#include <Arduino.h>
#include "graphics_init.h"
#include "graphics.h"
#include "touch_driver.h"
#include "WiFi_init.h"
#include "lvgl.h"
#include "xTimer.h"

#define TIMER_DIVIDER         (16)  //  Hardware timer clock divider
#define TIMER_SCALE           (TIMER_BASE_CLK / TIMER_DIVIDER)  // convert counter value to seconds

//----------------------------------------------------------------------------------------//
STATUS_t flags;
static hw_timer_t *timer1 = NULL;
//char dbg_buf [30];
static void vTimerContEventCallback(xTimerHandle xTimer); 
static xTimerHandle xTimerContEvent;

//---------------------------------------------------------------------------------------//
void IRAM_ATTR onTimer()
{
  systick_callback();
  lv_tick_inc(1);   //tick_period the call period of this function in milliseconds
}

//---------------------------------------------------------------------------------------//
void timerinit (void)
{
  timer1 = timerBegin(1, 80, true);  // Set timer frequency to 1Mhz
  timerSetAutoReload(timer1, true);
  timerAttachInterrupt(timer1, onTimer, true); // Attach onTimer function to our timer.
  timerAlarmWrite(timer1, 1000, true); // Set alarm to call onTimer function every second (value in microseconds)
  timerAlarmEnable(timer1);

  systick_init(&xTimer_task);
	xTimer_init(&get_systick);
  xTimerContEvent = xTimer_create(1000, true, &vTimerContEventCallback, true); 
}


//---------------------------------------------------------------------------------------//
void setup()
{
  Serial.begin(115200);

  flags.status = 0;

  gt911_int_();
  lvgl_init ();
  timerinit();
  input_driver_init ();

  screens_init ();
  //init_WiFi_connection ();
  
}

//---------------------------------------------------------------------------------------//
void loop() 
{

  //WiFi_connection_check ();
  lv_task_handler();
  delay (5);
}

//---------------------------------------------------------------------------------------//
static void vTimerContEventCallback(xTimerHandle xTimer)
{
  time_update();
}