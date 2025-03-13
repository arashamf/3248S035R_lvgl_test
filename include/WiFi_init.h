#ifndef WiFi_init_h
#define WiFi_init_h

//-------------------------------------------------------------------------------//
#include "typedef.h"
#include "NTPClient.h"

//-------------------------------------------------------------------------------//
void init_WiFi_connection (void);
void get_NTP_time (void);
void time_update (void);
void WiFi_connection_check (void);
void switch_net_blueled (uint8_t comm);
void switch_net_greenled (uint8_t comm);

//-------------------------------------------------------------------------------//
extern char c_Time[];  //массив с данными времени формата чч:мм:сс, который будет передаваться для отображения на экране
extern char d_Time[]; //массив с данными даты формата дд.мм.гггг, который будет передаваться для отображения на экране
const char NTPserver1[] = "0.ru.pool.ntp.org";
const char NTPserver2[] = "1.ru.pool.ntp.org";
const char NTPserver3[] = "ntp2.vniiftri.ru";
const char NTPserver4[] = "ntp.ix.ru";

//----------------------------------------------------------------------------//
class NTPtimedata
{
    public:
        NTPtimedata (NTPClient &ptr_NTP, char * buf, void (*put_data_scr)(char * ) ) 
        {
            sec = minute = hour = day = month = year = 0;
            UNIX_time = 0;
            _buf = buf;
            this->_ptr_NTP =  &ptr_NTP;
            _put_data_scr = put_data_scr;
        }
        int sec;
        int minute;
        int hour;
        int day; 
        int month;
        int year;    

        void getTimeData();
        void time_inc ();


    private:
        char * _buf; //указатель на массив с данными времени, который будет передаваться для отображения на экране
        NTPClient * _ptr_NTP ;
        uint64_t UNIX_time;
        void convert_time (void);
        void (*_put_data_scr)(char * ); //указатель на функцию отправки данных времени на экран
};
#endif 