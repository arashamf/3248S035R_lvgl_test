#ifndef graphics_init_h
#define graphics_init_h

//----------------------------------------------------------------------------//
#include "lvgl.h"

//----------------------------------------------------------------------------//
#define     LANDSCAPE

//----------------------------------------------------------------------------//
enum display_orientation
{
    portraite_1 =      0,
    landscape_1         ,
    portraite_2         ,
    landscape_2 =      3
};

//----------------------------------------------------------------------------//
void my_disp_flush(lv_disp_drv_t *, const lv_area_t *, lv_color_t *);
void lvgl_init (void);
void input_driver_init (void);

#endif 

