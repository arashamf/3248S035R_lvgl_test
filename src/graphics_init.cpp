//-------------------------------------------------------------------------------//
#include "TFT_eSPI.h"
#include "User_Setup.h"
#include "graphics_init.h"
#include "touch_driver.h"
#include "lvgl.h"

//-------------------------------------------------------------------------------//
#define      LV_USE_TFT_ESPI       1

//-------------------------------------------------------------------------------//
TFT_eSPI tft = TFT_eSPI();
static void tft_init (void);

//-------------------------------------------------------------------------------//
static const uint16_t screenWidth  = TFT_WIDTH;
static const uint16_t screenHeight = TFT_HEIGHT;

static lv_disp_draw_buf_t disp_buf;                   //LVGL stuff;
static lv_color_t buf[screenHeight * screenWidth / 8];
static lv_disp_drv_t disp_drv;
static lv_indev_drv_t indev_drv;

//-------------------------------------------------------------------------------//
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors(&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

//-------------------------------------------------------------------------------//
static void tft_init (void)
{
  tft.begin();               //TFT init 
  tft.setRotation(0);       //(Landscape) Portraite orientation 
  tft.fillScreen(TFT_RED);
  delay(100);
  tft.fillScreen(TFT_GREEN);
  delay(100);
  tft.fillScreen(TFT_BLUE);
  delay(100);
}

//-------------------------------------------------------------------------------//
void lvgl_init (void)
{
    tft_init ();
    lv_init();
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, screenHeight * 10);

    lv_disp_drv_init(&disp_drv);      //Initialize the display
    //disp_drv.hor_res = screenHeight;
   // disp_drv.ver_res = screenWidth;
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &disp_buf;
    lv_disp_drv_register(&disp_drv);
}

//-------------------------------------------------------------------------------//
void input_driver_init (void)
{
  lv_indev_drv_init( &indev_drv );
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register( &indev_drv );
}

