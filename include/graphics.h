#ifndef graphics_h
#define graphics_h

//----------------------------------------------------------------------------//
#include "typedef.h"
#include "lvgl.h"

//----------------------------------------------------------------------------//
void screens_init (void);
lv_obj_t *  main_menu_screen (lv_obj_t * scr);
lv_obj_t *  mars_menu_screen (lv_obj_t * scr);
lv_obj_t *  button_init (lv_obj_t * scr);

//----------------------------------------------------------------------------//
class visual_LED
{
    public:
        visual_LED (lv_obj_t * main_scr, lv_obj_t * led, lv_coord_t x_ofs, lv_coord_t y_ofs, lv_palette_t color_led)
        {
            scr = main_scr;
            led = lv_led_create(scr);
            kord_x = x_ofs;
            kord_y = y_ofs;
            color = lv_palette_main(color_led);
            _led = led;
            lv_obj_align(_led, LV_ALIGN_TOP_LEFT, kord_x, kord_y);
	        lv_obj_set_size(_led, 35, 35);
	        lv_led_set_brightness(_led, LV_LED_BRIGHT_MAX);
            lv_led_set_color(_led, color);
            switch_led (OFF);
        }
        void switch_led (uint8_t comm);
        void toogle_led (void);

    private:
        lv_obj_t * scr;
        int16_t kord_x;
        int16_t kord_y;
        lv_obj_t * _led;
        lv_color_t color;

};

//----------------------------------------------------------------------------//
class visual_button
{
    public:
        visual_button (lv_obj_t * main_scr, void (*lv_event_cb_t)(lv_event_t * e), 
        lv_coord_t size_x, lv_coord_t size_y, lv_coord_t x_ofs, lv_coord_t y_ofs, const char * title)
        {
            scr = main_scr;
            width_x = size_x;
            height_y = size_y;
            kord_x = x_ofs;
            kord_y = y_ofs;
            btn = lv_btn_create(scr);
            lv_obj_set_size(btn, width_x, height_y); 
            lv_obj_align(btn, LV_ALIGN_TOP_LEFT, kord_x, kord_y);
            lv_obj_add_event_cb (btn, lv_event_cb_t, LV_EVENT_CLICKED, NULL);
            lv_obj_t * label = lv_label_create(btn);
            lv_label_set_text(label, title);
            lv_obj_center(label);
        }

    private:
        lv_obj_t *  scr;
        lv_obj_t *  btn;
        int16_t     width_x;
        int16_t     height_y;
        int16_t     kord_x;
        int16_t     kord_y;
};

//----------------------------------------------------------------------------//
extern visual_LED * Bled;
extern visual_LED * Gled;

#endif 