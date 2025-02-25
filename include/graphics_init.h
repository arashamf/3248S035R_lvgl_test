#ifndef graphics_init_h
#define graphics_init_h

//----------------------------------------------------------------------------//
#include "lvgl.h"

//----------------------------------------------------------------------------//
void my_disp_flush(lv_disp_drv_t *, const lv_area_t *, lv_color_t *);
void lvgl_init (void);
void input_driver_init (void);

#endif 

