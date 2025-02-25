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
    main_screen    =   0,
    mcd_screen     =   1,
};


//---------------------------------------------------------------//
typedef union 
{	
	struct 
	{
		uint8_t							: 7;
		uint8_t flag_button 			: 1; 
	};
	uint8_t FlagStatus;
} STATUS_t;

#endif 