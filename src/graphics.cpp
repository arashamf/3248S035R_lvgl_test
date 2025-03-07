#include "graphics.h"
#include "User_Setup.h"
#include "typedef.h"
#include "WiFi_init.h"
#include "lvgl.h"
#include "../examples/lv_examples.h"

//-------------------------------------------------------------------------//
#define CENTER_VER   (TFT_HEIGHT/2)
#define CENTER_HOR   (TFT_WIDTH/2)

//------------------------------------------------------------------------//
static lv_style_t             bigStyle;
static lv_obj_t  *            main_menu_scr;
static lv_obj_t  *            clock_scr;
static lv_obj_t  *            main_label;
static lv_obj_t  *            scr_time_label;
static lv_obj_t  *            led_blue;
static lv_obj_t  *            led_green;
static lv_obj_t  *            meter;
static lv_obj_t  *            time_win;
static lv_obj_t  *            c_label;
visual_LED *                  Bled;
visual_LED *                  Gled;
visual_button *               BlueBtn;
visual_button *               GreenBtn;
visual_button *               ToogleMarsScrBtn;
visual_button *               ToogleSetupScrBtn;
visual_button *               ToogleInitWiFiBtn;
visual_button *               GetNTPtimeBtn;

//------------------------------------------------------------------------//
char label1 []= "BLUE";
char label2 []= "GREEN";
char label3 []= "MTC";  //Марсианское координированное время
char label4 []= "MainMenu";  
char label5 []= "WifiInit"; 
char label6 []= "Get time"; 

//------------------------------------------------------------------------//
uint8_t size_x = 90;
uint8_t size_y = 50;
uint8_t koord_x = 50;
uint8_t koord_y = 100;
//------------------------------------------------------------------------//
static void style_init (lv_style_t * Style);
lv_obj_t *  main_menu_screen_init (lv_obj_t * );
lv_obj_t *  time_screen_init (lv_obj_t * );
static void event_handler_get_NTP_time(lv_event_t * );
static void event_handler_switch_btn(lv_event_t * );
static void event_handler_btn_Bled(lv_event_t *);
static void event_handler_btn_Gled(lv_event_t * );
static void event_handler_wifi_btn(lv_event_t * );
static void clock_meter_init (lv_obj_t * );
static void set_value(void * , int32_t ); //коллбэк анимации циферблата часов
static lv_obj_t * lv_win_time(lv_obj_t * , lv_obj_t * , lv_obj_t *);

//------------------------------------------------------------------------//
void screens_init (void)
{
    main_menu_scr = lv_scr_act(); //Get the active screen of the default display
    main_menu_scr = main_menu_screen_init (main_menu_scr);
    clock_scr = time_screen_init (clock_scr);
}

//------------------------------------------------------------------------//
static void style_init (lv_style_t * Style)
{
    lv_style_init(Style);
    lv_style_set_text_font(Style, &lv_font_montserrat_28);
    
}

//------------------------------------------------------------------------//
lv_obj_t *  main_menu_screen_init (lv_obj_t * scr)
{
    main_label = lv_label_create(scr);
    lv_label_set_long_mode(main_label, LV_LABEL_LONG_WRAP);
    lv_label_set_text(main_label, "MAIN MENU");
    lv_obj_set_size(main_label, 240, 40);
    lv_obj_set_pos(main_label, CENTER_HOR-80, 15);
    //lv_obj_set_x(main_label, LV_ALIGN_OUT_TOP_MID);
   // lv_obj_set_y(main_label, 25);
    style_init (&bigStyle);
    lv_obj_add_style(main_label, &bigStyle, 0);


    flags.status_btn_scr = main_screen; //main_screen == 0

    BlueBtn = new visual_button (scr, event_handler_btn_Bled, size_x, size_y, 5, 
    CENTER_VER-3*size_y, label1);

    GreenBtn = new visual_button (scr, event_handler_btn_Gled, size_x, size_y, 
    CENTER_HOR+65, CENTER_VER-3*size_y, label2);

    ToogleMarsScrBtn = new visual_button (scr, event_handler_switch_btn, size_x, size_y, 
    CENTER_HOR-size_x/2, CENTER_VER-3*size_y, label3);

    clock_meter_init (scr);

    Bled = new visual_LED (scr, led_blue, koord_x/2, koord_y/2, LV_PALETTE_BLUE);
    Gled = new visual_LED (scr, led_green, CENTER_HOR+(2*koord_x), koord_y/2, LV_PALETTE_GREEN);;
    Gled->toogle_led ();
    Bled->toogle_led ();
    return scr; 
}

//-----------------------------------------------------------------------//
lv_obj_t *  time_screen_init (lv_obj_t * scr)
{
    scr = lv_obj_create (NULL); 
    scr_time_label = lv_label_create(scr);
    lv_label_set_long_mode(scr_time_label, LV_LABEL_LONG_WRAP);
    lv_label_set_text(scr_time_label, "TIME CLOCK");
    lv_obj_set_size(scr_time_label, 240, 40);
    lv_obj_set_pos(scr_time_label, CENTER_HOR-80, 5);

    lv_obj_add_style(scr_time_label, &bigStyle, 0);

    ToogleSetupScrBtn = new visual_button (scr, event_handler_switch_btn, size_x, size_y, 
    5, CENTER_VER-4*size_y, label4);

    ToogleInitWiFiBtn = new visual_button (scr, event_handler_wifi_btn, size_x, size_y, 
    CENTER_HOR-size_x/2, CENTER_VER-4*size_y, label5);

    GetNTPtimeBtn = new visual_button (scr, event_handler_get_NTP_time, size_x, size_y, 
    CENTER_HOR+65, CENTER_VER-4*size_y, label6);

    c_label = lv_win_time(scr, time_win, c_label);
    return scr; 
}

//-----------------------------------------------------------------------//
static void event_handler_get_NTP_time(lv_event_t * event)
{
	lv_obj_t *  button = lv_event_get_target(event);
	lv_obj_t * label = lv_obj_get_child(button, 0);
    get_NTP_time ();
}

//-----------------------------------------------------------------------//
static void event_handler_switch_btn(lv_event_t * event)
{
	lv_obj_t *  button = lv_event_get_target(event);
	lv_obj_t * label = lv_obj_get_child(button, 0);
	flags.status_btn_scr = !flags.status_btn_scr;
    switch (flags.status_btn_scr)
    {
        case mcd_screen:
            lv_scr_load(clock_scr);
            break;

        case main_screen:
            lv_scr_load(main_menu_scr);
            break;
    }
}
//-----------------------------------------------------------------------//
static void event_handler_btn_Bled( lv_event_t * event)
{
    lv_obj_t * btn = lv_event_get_target(event);
    lv_obj_t * label = lv_obj_get_child(btn, 0);
    Bled->toogle_led();
}

//------------------------------------------------------------------------//
static void event_handler_btn_Gled(lv_event_t * event)
{
	lv_obj_t *  btn = lv_event_get_target(event);
	lv_obj_t * label = lv_obj_get_child(btn, 0);
    Gled->toogle_led();
}

//------------------------------------------------------------------------//
static void event_handler_wifi_btn(lv_event_t * event)
{
	lv_obj_t *  btn = lv_event_get_target(event);
	lv_obj_t * label = lv_obj_get_child(btn, 0);
    init_WiFi_connection ();
}

//------------------------------------------------------------------------//
void visual_LED::switch_led (uint8_t comm)
{
    if (comm == ON)
    {   lv_led_on(_led); }
    else 
    {
        if (comm == OFF)
        {   lv_led_off(_led);   }
    }
}

//------------------------------------------------------------------------//
void visual_LED::toogle_led (void)
{
    lv_led_toggle(_led);
}

//------------------------------------------------------------------------//
static void set_value(void * indic, int32_t v)
{
    lv_meter_set_indicator_end_value(meter, (lv_meter_indicator_t *)indic, v);
}

//------------------------------------------------------------------------//
static void clock_meter_init (lv_obj_t * scr)
{
    meter = lv_meter_create(scr);
    lv_obj_set_size(meter, 250, 250);
    lv_obj_align (meter, LV_ALIGN_BOTTOM_MID, 0, -20);

    //Create a scale for the minutes
    //61 ticks in a 360 degrees range (the last and the first line overlaps)
    lv_meter_scale_t * scale_min = lv_meter_add_scale(meter);
    lv_meter_set_scale_ticks(meter, scale_min, 61, 1, 10, lv_palette_main(LV_PALETTE_GREY));
    lv_meter_set_scale_range(meter, scale_min, 0, 60, 360, 270);

    //create another scale for the hours. It's only visual and contains only major ticks
    lv_meter_scale_t * scale_hour = lv_meter_add_scale(meter);
    lv_meter_set_scale_ticks(meter, scale_hour, 12, 0, 0, lv_palette_main(LV_PALETTE_GREY)); //12 ticks
    lv_meter_set_scale_major_ticks(meter, scale_hour, 1, 2, 20, lv_color_black(), 10); //Every tick is major
    lv_meter_set_scale_range(meter, scale_hour, 1, 12, 330, 300);       //[1..12] values in an almost full circle
    
    static lv_meter_indicator_t * sec_indic;
    sec_indic = lv_meter_add_scale_lines(meter, scale_min, lv_palette_main(LV_PALETTE_GREY), 
    lv_palette_main(LV_PALETTE_BLUE), false, 0);
    lv_meter_set_indicator_start_value(meter, sec_indic, 0);
    lv_meter_set_indicator_end_value(meter, sec_indic, 20);
    sec_indic = lv_meter_add_needle_line(meter, scale_min, 4, lv_palette_main(LV_PALETTE_GREY), -10);

    static lv_meter_indicator_t * min_indic;
    min_indic = lv_meter_add_scale_lines(meter, scale_min, lv_palette_main(LV_PALETTE_GREY), 
    lv_palette_main(LV_PALETTE_BLUE), false, 0);
    lv_meter_set_indicator_start_value(meter, min_indic, 0);
    lv_meter_set_indicator_end_value(meter, min_indic, 15);
    min_indic = lv_meter_add_needle_line(meter, scale_min, 5, lv_palette_main(LV_PALETTE_GREEN), -15);

    static lv_meter_indicator_t * hour_indic;
    hour_indic = lv_meter_add_scale_lines(meter, scale_min, lv_palette_main(LV_PALETTE_GREY), 
    lv_palette_main(LV_PALETTE_BLUE), false, 0);
    lv_meter_set_indicator_start_value(meter, hour_indic, 0);
    lv_meter_set_indicator_end_value(meter, hour_indic, 15);
    hour_indic = lv_meter_add_needle_line(meter, scale_min, 6, lv_palette_main(LV_PALETTE_RED), -15);

    lv_anim_t a;        //Create an animation to set the value
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, set_value);
    lv_anim_set_values(&a, 0, 60);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_time(&a, 60*1000);    //таймер увеличивается раз в 1 с
    lv_anim_set_var(&a, sec_indic);
    lv_anim_start(&a);

    lv_anim_set_var(&a, min_indic);
    lv_anim_set_time(&a, 60*60*1000); 
    lv_anim_set_values(&a, 0, 60);
    lv_anim_start(&a);

    lv_anim_set_var(&a, hour_indic);
    lv_anim_set_time(&a, 12*60*60*1000);    
    lv_anim_set_values(&a, 0, 60);
    lv_anim_start(&a);
}

//------------------------------------------------------------------------//
static lv_obj_t * lv_win_time(lv_obj_t * scr, lv_obj_t  * time_win, lv_obj_t * c_label)
{
    time_win = lv_win_create(scr, 40);
    lv_obj_set_size (time_win, 200, 300);
    lv_obj_align (time_win, LV_ALIGN_BOTTOM_MID, 0, -20);

    lv_obj_t * head = lv_win_get_header(time_win);
    lv_obj_t * t_label = lv_label_create(head);
    lv_label_set_text(t_label, "TIME CLOCK");

    static lv_style_t Style;
    lv_style_init(&Style);
    lv_style_set_text_font(&Style, &lv_font_montserrat_20);
    lv_obj_add_style(main_label, &Style, 0);

    lv_obj_t * cont = lv_win_get_content(time_win);  //Content can be added here
    c_label = lv_label_create(cont);
    lv_label_set_text_static (c_label, "no_time\n");
    lv_obj_add_style(c_label, &Style, 0);
    return c_label;
}

//------------------------------------------------------------------------//
void time_data_update (const char * text)
{
   // lv_label_set_text_fmt(c_label, text);
    lv_label_set_text_static (c_label, text);
}
