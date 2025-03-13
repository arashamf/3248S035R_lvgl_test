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
static lv_style_t             midFontStyle;
static lv_style_t             BigStyle;

static lv_obj_t  *            main_menu_scr;
static lv_obj_t  *            setup_scr;

static lv_obj_t  *            meter;
static lv_obj_t  *            time_win;
static lv_obj_t  *            c_label;

visual_LED *                  WiFiLed;
visual_LED *                  NTPLed;

set_label  *                  main_label;
set_label  *                  clock_label;
set_label  *                  setup_scr_label;

visual_button *               BlueBtn;
visual_button *               GreenBtn;
visual_button *               ToogleSetupScrBtn1;
visual_button *               ToogleSetupScrBtn2;
visual_button *               ToogleInitWiFiBtn;
visual_button *               GetNTPtimeBtn;

text_area     *               ClockArea;
text_area     *               statusWiFi;
text_area     *               statusNTP;
//------------------------------------------------------------------------//
char label1 []= "MainMenu";  
char label2 []= "Setup";  //Марсианское координированное время
char label3 []= "WifiInit"; 
char label4 []= "Get time"; 

const char label_main_src []= "Main Menu"; 
const char label_setup_src []= "Setup Menu"; 
//------------------------------------------------------------------------//
uint8_t size_x = 90;
uint8_t size_y = 50;

//------------------------------------------------------------------------//
static void style_init (lv_style_t * , const lv_font_t * );
static lv_obj_t *  main_menu_screen_init (void);
static lv_obj_t *  time_screen_init (void);
static void event_handler_get_NTP_time(lv_event_t * );
static void event_handler_switch_btn(lv_event_t * );
static void event_handler_wifi_btn(lv_event_t * );

static void clock_meter_init (lv_obj_t * );
static void set_value(void * , int32_t ); //коллбэк анимации циферблата часов

//------------------------------------------------------------------------//
void screens_init (void)
{
    main_menu_scr = main_menu_screen_init ();
    setup_scr = time_screen_init ();
}

//------------------------------------------------------------------------//
static void style_init (lv_style_t * Style, const lv_font_t * font_value)
{
    lv_style_init(Style);
    lv_style_set_text_font(Style, font_value);
    
    
}

//------------------------------------------------------------------------//
static lv_obj_t * main_menu_screen_init (void)
{
    lv_obj_t *  scr = lv_obj_create(NULL); //Create a base object
    scr = lv_scr_act(); //Get the active screen of the default display
    flags.status_btn_scr = main_screen; //main_screen == 0

    style_init (&midFontStyle, &lv_font_montserrat_20);
    style_init (&BigStyle, &lv_font_montserrat_28);
    main_label = new set_label (scr, 150, 40, LV_ALIGN_TOP_MID, 25, 5, &midFontStyle, label_main_src);

    ToogleSetupScrBtn1 = new visual_button (scr, event_handler_switch_btn, size_x, size_y, 
    LV_ALIGN_TOP_MID, 0, 50, label2);

    clock_label = new set_label (scr, 200, 50, LV_ALIGN_CENTER, 40, 5, &BigStyle,  "01:00:00");
    //clock_meter_init (scr); //инициализация анимации часов
    return scr; 
}

//-----------------------------------------------------------------------//
static lv_obj_t *  time_screen_init (void)
{
    lv_obj_t *  scr = lv_obj_create (NULL); 

    setup_scr_label = new set_label (scr, 240, 40, LV_ALIGN_TOP_MID, 20, 5, &midFontStyle, label_setup_src);

    ToogleSetupScrBtn2 = new visual_button (scr, event_handler_switch_btn, size_x, size_y, 
    LV_ALIGN_TOP_LEFT, 5, 40, label1);

    ToogleInitWiFiBtn = new visual_button (scr, event_handler_wifi_btn, size_x, size_y, 
    LV_ALIGN_TOP_MID, 0, 40, label3);

    GetNTPtimeBtn = new visual_button (scr, event_handler_get_NTP_time, size_x, size_y, 
    LV_ALIGN_TOP_RIGHT, 0, 40, label4);

    WiFiLed = new visual_LED (scr, LV_ALIGN_LEFT_MID, 0, 0);
    statusWiFi = new text_area (scr, LV_ALIGN_LEFT_MID, 40, 0, lv_pct(33), "status WiFi");
    NTPLed = new visual_LED (scr, LV_ALIGN_LEFT_MID, 0, size_y+10);
    statusNTP = new text_area (scr, LV_ALIGN_LEFT_MID, 40, size_y+10, lv_pct(33), "status NTP");
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
            lv_scr_load(setup_scr);
            break;

        case main_screen:
            lv_scr_load(main_menu_scr);
            break;
    }
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
void visual_LED::led_status (uint8_t status)
{
    lv_color_t color;
    if (status == true)
    {   lv_led_set_color(_led, green_color);  }
    else
    {   lv_led_set_color(_led, red_color);    }
    
    switch_led (ON);
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
    lv_obj_align (meter, LV_ALIGN_BOTTOM_MID, 0, -20); //изменение начала координат и ввод новых координат

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
void set_label::setup_new_text (const char * new_txt)
{
    _txt = new_txt;
    lv_label_set_text_static (scr_label, _txt);
}

//------------------------------------------------------------------------//
void time_data_update (char * new_text)
{
   //lv_label_set_text_static (c_label, text);
   clock_label->setup_new_text (new_text);
}
