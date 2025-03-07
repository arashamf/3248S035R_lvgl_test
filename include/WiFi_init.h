#ifndef WiFi_init_h
#define WiFi_init_h

//-------------------------------------------------------------------------------//
#include "typedef.h"
#include "NTPClient.h"

//-------------------------------------------------------------------------------//
void init_WiFi_connection (void);
void get_NTP_time (void);
void WiFi_connection_check (void);
void switch_net_blueled (uint8_t comm);
void switch_net_greenled (uint8_t comm);

//-------------------------------------------------------------------------------//
extern const char * c_Time;
extern const char * d_Time;
const char NTPserver1[] = "0.ru.pool.ntp.org";
const char NTPserver2[] = "1.ru.pool.ntp.org";
const char NTPserver3[] = "ntp2.vniiftri.ru";
const char NTPserver4[] = "ntp.ix.ru";

//----------------------------------------------------------------------------//
class NTPtimedata
{
    NTPtimedata (NTPClient * ptr_NTP) 
    {
        sec = minute = hour = day = month = year = 0;

    }

    public:
        int sec;
        int minute;
        int hour;
        int day; 
        int month;
        int year;        
        NTPClient * ptr_NTP;
        void getTimeData();

    private:
        uint64_t UNIX_time = ptr_NTP->getEpochTime();
};
#endif 