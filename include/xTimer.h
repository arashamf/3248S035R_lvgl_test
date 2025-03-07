//-------------------------------------------------------------------------------//
#include "Arduino.h"

//-------------------------------------------------------------------------------//
#define     MAX_xTIMERS     3

//Exported types -----------------------------------------------------------------//
typedef 	void * xTimerHandle;
typedef void (*tmrTIMER_CALLBACK)(xTimerHandle xTimer);	
typedef void (*SysTick_CALLBACK)(uint32_t);
typedef uint32_t (*portTick)(void);

//-------------------------------------------------------------------------------//
typedef enum 
{
	__IDLE = 0, 
	__ACTIVE, 
	__DONE
} tmrTIMER_STATE;

//-------------------------------------------------------------------------------//
typedef struct
{
	uint32_t expiry;
  	uint32_t periodic;
	tmrTIMER_STATE State;
	bool AutoReload;
	tmrTIMER_CALLBACK	CallbackFunction;
} xTIMER;	

//-------------------------------------------------------------------------------//
uint32_t get_systick(void);
void systick_callback(void);
void systick_init(SysTick_CALLBACK );
void xTimer_init(uint32_t (*GetSysTick)(void));
xTimerHandle xTimer_create(uint32_t xTimerPeriodInTicks, bool AutoReload, tmrTIMER_CALLBACK CallbackFunction, bool NewState);
void xTimer_setperiod(xTimerHandle xTimer, uint32_t xTimerPeriodInTicks);
void xTimer_reload(xTimerHandle xTimer);
void xTimer_delete(xTimerHandle xTimer);
void xTimer_task(uint32_t portTick);
