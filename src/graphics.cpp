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
static lv_style_t           midFontStyle;
static lv_style_t           BigStyle;

static lv_obj_t  *          main_menu_scr; //главный экран
static lv_obj_t  *          wifi_id_setup_scr; //экран настройки логина пароля WiFi
static lv_obj_t  *          ip_setup_scr;

static lv_obj_t  *          wifi_id_kb;  //объект клавиатуры

visual_LED *                WiFiLed;
visual_LED *                NTPLed;

set_label  *                main_label;
set_label  *                clock_label;
set_label  *                setup_scr_label;
set_label  *                lbl_passwd;
set_label  *                lbl_login;
set_label  *                lbl_ip_area;

visual_button *             ToogleSetupScrBtn1;
visual_button *             ToogleSetupScrBtn2;
visual_button *             ToogleSetupScrBtn3;
visual_button *             ToogleInitWiFiBtn;
visual_button *             GetNTPtimeBtn;
visual_button *             ToogleSetIPScrBtn;

text_area     *             statusWiFi;
text_area     *             statusNTP;
text_area     *             passwd_area;
text_area     *             login_area;
text_area     *             setup_ip_area;

static lv_obj_t  *          dd;
static lv_obj_t  *          meter; //объект циферблат часов

//------------------------------------------------------------------------//
char label1 []= "MainMenu";  
char label2 []= "Setup";  //Марсианское координированное время
char label3 []= "WifiInit"; 
char label4 []= "Get time"; 
char label5 []= "IP setup"; 
char label6 []= "Back"; 

const char label_main_src []= "Main Menu"; 
const char label_setup_src []= "Setup Menu"; 
//------------------------------------------------------------------------//
uint8_t size_x = 90;
uint8_t size_y = 50;

//------------------------------------------------------------------------//
static void style_init (lv_style_t * , const lv_font_t * );
static lv_obj_t *  main_menu_screen_init (void);
static lv_obj_t *  setup_screen_init (void);
static lv_obj_t *  setupIP_screen_init (void);

static void event_handler_get_NTP_time(lv_event_t * );
static void event_handler_switch_btn(lv_event_t * );
static void event_handler_switch2_btn(lv_event_t * );
static void event_handler_wifi_btn(lv_event_t * );

static void passwd_event_cb(lv_event_t * );
static void login_event_cb(lv_event_t * );
static void setup_ip_event_cb(lv_event_t * );

void lv_dropdown (lv_obj_t * scr);

//------------------------------------------------------------------------//
void screens_init (void)
{
    main_menu_scr = main_menu_screen_init ();
    wifi_id_setup_scr = setup_screen_init ();
    ip_setup_scr = setupIP_screen_init ();
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
    flags.status_btn_scr = main; //main_screen == 0

    style_init (&midFontStyle, &lv_font_montserrat_20);
    style_init (&BigStyle, &lv_font_montserrat_28);

    ToogleSetupScrBtn1 = new visual_button (scr, event_handler_switch_btn, size_x, size_y, 
    LV_ALIGN_TOP_LEFT, 0, 10, label2);

    ToogleInitWiFiBtn = new visual_button (scr, event_handler_wifi_btn, size_x, size_y, 
    LV_ALIGN_TOP_MID, 0, 10, label3);

    GetNTPtimeBtn = new visual_button (scr, event_handler_get_NTP_time, size_x, size_y, 
    LV_ALIGN_TOP_RIGHT, 0, 10, label4);

    WiFiLed = new visual_LED (scr, LV_ALIGN_TOP_LEFT, 0, 75);
    statusWiFi = new text_area (scr,  NULL, false,  "status WiFi", 22, LV_ALIGN_TOP_LEFT, 40, 75);

    NTPLed = new visual_LED (scr, LV_ALIGN_TOP_MID, 0, 75);
    statusNTP = new text_area (scr, NULL, false, "status NTP", 22, LV_ALIGN_TOP_MID,  75, 75);

    clock_label = new set_label (scr, 200, 50, LV_ALIGN_CENTER, 40, 5, &BigStyle,  c_Time);
    return scr; 
}

//-----------------------------------------------------------------------//
static lv_obj_t *  setup_screen_init (void)
{
    lv_obj_t *  scr = lv_obj_create (NULL); 
    lv_coord_t x_ofs = 0;
    lv_coord_t y_ofs = 100;

    ToogleSetupScrBtn2 = new visual_button (scr, event_handler_switch_btn, size_x, size_y, 
    LV_ALIGN_TOP_LEFT, 0, 10, label1);

    ToogleSetIPScrBtn = new visual_button (scr, event_handler_switch2_btn, size_x, size_y, 
    LV_ALIGN_TOP_MID, 0, 10, label5);

    passwd_area = new text_area (scr,  passwd_event_cb, false,  net_setting.password , 40, LV_ALIGN_TOP_RIGHT, x_ofs, y_ofs);
    lbl_passwd = new set_label (scr, passwd_area->back_ta_ptr(), LV_ALIGN_OUT_TOP_LEFT, 5, 0, "password");
    login_area = new text_area  (scr,  login_event_cb, false,  net_setting.ssid, 40, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);
    lbl_login = new set_label (scr, login_area->back_ta_ptr(), LV_ALIGN_OUT_TOP_LEFT, 5, 0, "ssid");

    wifi_id_kb = lv_keyboard_create(scr); // Create a keyboard
    lv_obj_set_size(wifi_id_kb, LV_HOR_RES, LV_VER_RES / 2);

    return scr; 
}

//-----------------------------------------------------------------------//
static lv_obj_t *  setupIP_screen_init (void)
{
    lv_coord_t x_ofs = 0;
    lv_coord_t y_ofs = 100;

    lv_obj_t *  scr = lv_obj_create (NULL); 

    ToogleSetupScrBtn3 = new visual_button (scr, event_handler_switch2_btn, size_x, size_y, 
    LV_ALIGN_TOP_LEFT, 0, 10, label6);

    setup_ip_area = new text_area (scr,  passwd_event_cb, false,  net_setting.ip , 33, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);
    lbl_ip_area = new set_label (scr, setup_ip_area->back_ta_ptr(), LV_ALIGN_OUT_TOP_LEFT, 5, 0, "ip adress");

    lv_dropdown (scr);

    return scr; 

}

//-----------------------------------------------------------------------//
static void event_handler_get_NTP_time(lv_event_t * event)
{
	lv_obj_t *  button = lv_event_get_target(event);
	lv_obj_t * label = lv_obj_get_child(button, 0);
    ptr_ntp_data->getTimeData();
}

//-----------------------------------------------------------------------//
static void event_handler_switch_btn(lv_event_t * event)
{
	lv_obj_t *  button = lv_event_get_target(event);
	lv_obj_t * label = lv_obj_get_child(button, 0);

    switch (flags.status_btn_scr)
    {
        case wifi_id_setup:
            flags.status_btn_scr = main;
            lv_scr_load(main_menu_scr);
            break;

        case main:
            flags.status_btn_scr = wifi_id_setup;
            lv_scr_load(wifi_id_setup_scr);
            break;
    }
}

//-----------------------------------------------------------------------//
static void event_handler_switch2_btn(lv_event_t * event)
{
	lv_obj_t *  button = lv_event_get_target(event);
	lv_obj_t * label = lv_obj_get_child(button, 0);

    switch (flags.status_btn_scr)
    {
        case ip_setup:
            flags.status_btn_scr = wifi_id_setup;
            lv_scr_load(wifi_id_setup_scr);
            break;

        case wifi_id_setup:
            flags.status_btn_scr = ip_setup;
            lv_scr_load(ip_setup_scr);
            break;

        default:
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
void set_label::setup_new_text (const char * new_txt)
{
    _txt = new_txt;
    lv_label_set_text_static (scr_label, _txt);
}

//------------------------------------------------------------------------//
void time_data_update (char * new_text)
{
   clock_label->setup_new_text (new_text);
}

//------------------------------------------------------------------------//
static void set_value(void * indic, int32_t v)
{
    lv_meter_set_indicator_end_value(meter, (lv_meter_indicator_t *)indic, v);
}

//------------------------------------------------------------------------//
void text_area :: setup_new_text (const char * txt)
{
    _txt = txt;
    lv_textarea_set_text (ta, _txt);
}

//------------------------------------------------------------------------//
void set_ip_area(char * new_text)
{
    setup_ip_area->setup_new_text (new_text);
}

//------------------------------------------------------------------------//
static void login_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED || code == LV_EVENT_FOCUSED) 
    {       
        if(wifi_id_kb != NULL) 
        {   lv_keyboard_set_textarea(wifi_id_kb, ta);  }    //Focus on the clicked text area
    }
    else 
    {
        if(code == LV_EVENT_READY) 
        {   
            net_setting.replace_ssid (lv_textarea_get_text(ta));
        }
    }
}

//------------------------------------------------------------------------//
static void passwd_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED || code == LV_EVENT_FOCUSED) 
    {       
        if(wifi_id_kb != NULL) 
        {   lv_keyboard_set_textarea(wifi_id_kb, ta);  }    //Focus on the clicked text area
    }
    else 
    {
        if(code == LV_EVENT_READY) 
        {   
            net_setting.replace_password (lv_textarea_get_text(ta));
          //  Serial.printf("Ready, current password: %s\r\n", net_setting.password); 
        }
    }
}

//------------------------------------------------------------------------//
static void setup_ip_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED || code == LV_EVENT_FOCUSED) 
    {       
        if(wifi_id_kb != NULL) 
        {   lv_keyboard_set_textarea(wifi_id_kb, ta);  } //Focus on the clicked text area
    }
    else 
    {
        if(code == LV_EVENT_READY) 
        {   
            net_setting.replace_password (lv_textarea_get_text(ta));
        }
    }
}

//------------------------------------------------------------------------//
static void event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) 
    {
        char buf[32];
        lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
        Serial.println(buf);       
    }
}

//------------------------------------------------------------------------//
void lv_dropdown (lv_obj_t * scr)
{

    dd = lv_dropdown_create(scr);  /*Create a normal drop down list*/
    lv_dropdown_set_options(dd, "0.ru.pool.ntp.org\n"
                            "1.ru.pool.ntp.org\n"
                            "ntp0.NL.net\n"
                            "ntp2.vniiftri.ru\n"
                            "ntp.ix.ru\n"
                            "ntps1-1.cs.tu-berlin.de");

    lv_obj_align(dd,    LV_ALIGN_TOP_MID, 0, 20);
    lv_obj_add_event_cb(dd, event_handler, LV_EVENT_ALL, NULL);
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



