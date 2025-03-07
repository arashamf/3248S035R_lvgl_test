//----------------------------------------------------------------------------------------//
#include "xTimer.h"
#include "Arduino.h"

//Private variables-----------------------------------------------------------------------//
static portTick xTimeNow; 
static xTIMER xTimerList[MAX_xTIMERS];
static SysTick_CALLBACK SysTick_CallbackFunction = NULL; 
static uint32_t TicksCounter = 0; 



//--------------------------------------------------------------------------------------//
void systick_callback(void)
{
	TicksCounter++; 
	if ( SysTick_CallbackFunction != NULL ) 
	{SysTick_CallbackFunction(TicksCounter);} 
}

//--------------------------------------------------------------------------------------//
uint32_t get_systick(void)
{
	return TicksCounter;
}

//-------------------------------------------------------------------------------------//
void systick_init(SysTick_CALLBACK CallbackFunction)
{
	SysTick_CallbackFunction = CallbackFunction;
}

//-------------------------------------------------------------------------------------//
void xTimer_init	(uint32_t (*GetSysTick)(void))
{
	xTimeNow = GetSysTick; 
}

//------------------------------------------------------------------------------------//
xTimerHandle xTimer_create(uint32_t xTimerPeriodInTicks, bool AutoReload, 
tmrTIMER_CALLBACK CallbackFunction, bool NewState)
{
	xTimerHandle NewTimer = NULL; 
	uint16_t count;
	
	for (count = 0; count < MAX_xTIMERS; count++) 
	{
		if (xTimerList[count].CallbackFunction == NULL) 		
		{
			xTimerList[count].periodic = xTimerPeriodInTicks;		
			xTimerList[count].AutoReload = AutoReload;				
			xTimerList[count].CallbackFunction = CallbackFunction;	
			
			if (NewState != false) 
			{
				xTimerList[count].expiry = xTimeNow() + xTimerPeriodInTicks; 
				xTimerList[count].State = __ACTIVE; 
			} 
			else 
			{	xTimerList[count].State = __IDLE;	}	
			NewTimer = (xTimerHandle)(count+1); 
			break;
    }
  }
	return NewTimer;
}

//------------------------------------------------------------------------------------//
void xTimer_setperiod(xTimerHandle xTimer, uint32_t xTimerPeriodInTicks) 
{
	if ( xTimer != NULL )  
	{	xTimerList[(uint32_t)xTimer-1].periodic = xTimerPeriodInTicks;	} 
}

//-----------------------------------------------------------------------------------//
void xTimer_reload(xTimerHandle xTimer) 
{
	if ( xTimer != NULL ) 
	{
		xTimerList[(uint32_t)xTimer-1].expiry = xTimeNow() + xTimerList[(uint32_t)xTimer-1].periodic; //óñòàíîâêà óñòàâêè òàéìåðà
		xTimerList[(uint32_t)xTimer-1].State = __ACTIVE; 
	}
}

//------------------------------------------------------------------------------------//
void xTimer_delete(xTimerHandle xTimer)
{
	if ( xTimer != NULL ) 
	{
		xTimerList[(uint32_t)xTimer-1].CallbackFunction = NULL;
		xTimerList[(uint32_t)xTimer-1].State = __IDLE;
		xTimer = NULL;
	}		
}

//------------------------------------------------------------------------------------//
void xTimer_task(uint32_t portTick)
{
	uint16_t i;
	
	for (i = 0; i < MAX_xTIMERS; i++) 
	{
		switch (xTimerList[i].State) 
		{
			case __ACTIVE: 
				if ( portTick >= xTimerList[i].expiry )
				{				
					if ( xTimerList[i].AutoReload != 0 ) 
					{	xTimerList[i].expiry = portTick + xTimerList[i].periodic;	} 
					else 
					{	xTimerList[i].State = __IDLE;	}	
					xTimerList[i].CallbackFunction((xTimerHandle)(i+1)); 
				}					
				break;
				
			default:
				break;
		}
	}	
}

