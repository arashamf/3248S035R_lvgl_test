#ifndef typedef_h
#define typedef_h

//---------------------------------------------------------------//
#include <Arduino.h>

//---------------------------------------------------------------//
#define     ON          1
#define     OFF         0
#define     GREENled    16  // Assign output variables to GPIO pins
#define     BLUEled     17

//---------------------------------------------------------------//
enum leds
{
    BlueLed    =   1,
    GreenLed   =   2,
};

//---------------------------------------------------------------//
enum Screens
{
    main    		=   	0,
    wifi_id_setup   =   	1,
	ip_setup   		=   	2,
};


//---------------------------------------------------------------//
typedef union 
{	
	struct 
	{
		uint8_t							: 3;
		uint8_t		NTP_start			: 1;
		uint8_t		status_NTP			: 1;
		uint8_t		status_WiFi			: 1;
		uint8_t 	status_btn_scr 		: 2; 
	};
	uint8_t status;
} STATUS_t;

//------------------------------------------------------------------------//
/*const char digital [] = {'0','\n','1','\n','2','\n','3','\n','4','\n','5','\n','6','\n','7','\n',
'8','\n','9'};*/

extern STATUS_t flags;

#endif 