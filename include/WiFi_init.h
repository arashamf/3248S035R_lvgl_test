#ifndef WiFi_init_h
#define WiFi_init_h

//-------------------------------------------------------------------------------//
#include "typedef.h"

//-------------------------------------------------------------------------------//
void init_WiFi_connection (void);
void WiFi_connection_check (void);
void switch_net_blueled (uint8_t comm);
void switch_net_greenled (uint8_t comm);


#endif 