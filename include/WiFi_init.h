#ifndef WiFi_init_h
#define WiFi_init_h

//-------------------------------------------------------------------------------//
#include "typedef.h"
#include "NTPClient.h"
#include <cstring>

//-------------------------------------------------------------------------------//
void init_WiFi_connection (void);
void check_WiFi_status (void);
void HTTP_connection_check (void);
void switch_net_blueled (uint8_t comm);
void switch_net_greenled (uint8_t comm);

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

//----------------------------------------------------------------------------//
class WiFidata
{
    public:
        WiFidata (const char * id, const char * pass, void (*put_ip_scr)(char * ))
        {
            len_ssid = std::strlen(id);
            ssid = new char [len_ssid + 1] ;
            std :: strcpy (ssid , id) ; // инициализация указателя
            len_password = std::strlen(pass);
            password = new char [len_password + 1] ;
            std :: strcpy (password, pass) ; // инициализация указателя
            sprintf (ip, "%u:%u:%u:%u", 192, 168, 0, 1);
            _put_ip_scr = put_ip_scr;
        }

        ~WiFidata ()
        {
            delete [] ssid;
            delete [] password;
        }

       // WiFidata & replace_ssid (char * ssid, const char * buf)
        void replace_ssid (const char * buf)
        {
            len_ssid = std::strlen(buf);
            if (len_ssid > 0)
            {
                delete [] ssid;
                ssid = new char [len_ssid + 1] ;
                std :: strcpy (ssid , buf) ;
            }
        }

        //WiFidata & replace_password (char * password, const char * buf)
        void replace_password (const char * buf)
        {
            len_password = std::strlen(buf);
            Serial.printf("len=%u\r\n", len_password); 
            if (len_password > 0)
            {
                delete [] password;
                password = new char [len_password + 1] ;
                std :: strcpy (password , buf) ;
            }
        }
        
        void set_ip (char * txt)
        {   _put_ip_scr(txt);   }

        char * ssid;
        char * password;
        char  ip[16];

    private:
        int len_ssid;
        int len_password;
        void (*_put_ip_scr)(char * );
};

//-------------------------------------------------------------------------------//
extern char c_Time[];  //массив с данными времени формата чч:мм:сс, который будет передаваться для отображения на экране
extern char d_Time[]; //массив с данными даты формата дд.мм.гггг, который будет передаваться для отображения на экране

extern NTPtimedata * ptr_ntp_data;
extern WiFidata net_setting;

const char NTPserver1[] = "0.ru.pool.ntp.org";
const char NTPserver2[] = "1.ru.pool.ntp.org";
const char NTPserver3[] = "ntp2.vniiftri.ru";
const char NTPserver4[] = "ntp.ix.ru";

#endif 