#ifndef graphics_h
#define graphics_h

//----------------------------------------------------------------------------//
#include "typedef.h"
#include "lvgl.h"

//----------------------------------------------------------------------------//
void screens_init (void);
void time_data_update (char * );

//----------------------------------------------------------------------------//
class visual_LED
{
    public:
        visual_LED (lv_obj_t * main_scr, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs)
        {
            scr = main_scr;
            _led = lv_led_create(scr);
            kord_x = x_ofs;
            kord_y = y_ofs;
            _align = align;
            lv_obj_align(_led, _align, kord_x, kord_y);
	        lv_obj_set_size(_led, 35, 35);
            green_color = lv_palette_main(LV_PALETTE_GREEN);
            red_color = lv_palette_main(LV_PALETTE_RED);
            grey_color = lv_palette_main(LV_PALETTE_GREY);
            lv_led_set_color(_led, grey_color);
            switch_led (OFF);
        }
        void switch_led (uint8_t comm);
        void toogle_led (void);
        void led_status (uint8_t status);

    private:
        lv_obj_t * scr;
        lv_obj_t * _led;
        lv_align_t _align;
        int16_t kord_x;
        int16_t kord_y;
        lv_color_t green_color;
        lv_color_t red_color;
        lv_color_t grey_color;

};

//----------------------------------------------------------------------------//
class visual_button
{
    public:
        visual_button (lv_obj_t * main_scr, void (*lv_event_cb_t)(lv_event_t * e), lv_coord_t size_x,
        lv_coord_t size_y, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs, const char * title)
        {
            _scr = main_scr;
            btn = lv_btn_create(_scr);
            width_x = size_x;
            height_y = size_y;
            lv_obj_set_size(btn, width_x, height_y); 
            _align = align;
            kord_x = x_ofs;
            kord_y = y_ofs;
            lv_obj_align(btn, _align, kord_x, kord_y);
            lv_obj_add_event_cb (btn, lv_event_cb_t, LV_EVENT_CLICKED, NULL);
            lv_obj_t * label = lv_label_create(btn);
            lv_label_set_text(label, title);
            lv_obj_center(label);
        }

    private:
        lv_obj_t *  _scr;
        lv_obj_t *  btn;
        int16_t     width_x;
        int16_t     height_y;
        lv_align_t  _align;
        int16_t     kord_x;
        int16_t     kord_y;
};

//----------------------------------------------------------------------------//
class text_area
{
    public:
        text_area (lv_obj_t * main_scr, lv_align_t align, lv_coord_t size_x,  lv_coord_t size_y, 
        lv_coord_t width, const char * txt)
        {
            _scr = main_scr;  
            ta = lv_textarea_create(_scr);  
            _align = align;
            _size_x = size_x;
            _size_y = size_y;
            lv_obj_align(ta, _align,  _size_x, _size_y); 
            lv_textarea_set_one_line(ta, true);
            _width = width;
            lv_obj_set_width(ta, _width);
            _txt = txt;
            lv_textarea_add_text(ta, _txt);
        }

    private:
        lv_obj_t *  _scr;
        lv_obj_t *  ta;
        lv_align_t  _align;
        lv_coord_t  _size_x;
        lv_coord_t  _size_y;
        lv_coord_t  _width;
        const char * _txt;    
};

//----------------------------------------------------------------------------//
class set_label
{
    public:
        set_label (lv_obj_t * main_scr, lv_coord_t size_x, lv_coord_t size_y, lv_align_t align,
        lv_coord_t x_ofs, lv_coord_t y_ofs, lv_style_t * style, const char * txt)
        {
            _scr = main_scr;  
            scr_label = lv_label_create(_scr);
            lv_label_set_long_mode(scr_label, LV_LABEL_LONG_WRAP);
            width = size_x;
            height = size_y;
            lv_obj_set_size(scr_label, width, height);
            kord_x = x_ofs;
            kord_y = y_ofs;
            lv_obj_align(scr_label, align, kord_x, kord_y);
            _txt = txt;
            //lv_label_set_text(scr_label, txt);
            lv_label_set_text_static (scr_label, _txt);
            _style = style;
            lv_obj_add_style(scr_label, _style, 0);
        }
        void setup_new_text (const char * txt);

    private:
        lv_obj_t *  _scr;
        lv_obj_t  * scr_label;
        int16_t     width;
        int16_t     height;
        lv_align_t  _align;
        int16_t     kord_x;
        int16_t     kord_y;
        lv_style_t * _style;
        const char * _txt;
};


//----------------------------------------------------------------------------//
extern visual_LED * WiFiLed;
extern visual_LED * NTPLed;

#endif 