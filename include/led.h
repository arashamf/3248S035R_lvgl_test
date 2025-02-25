
#ifndef led_h
#define led_h

#include "typedef.h"

class control_LED
{
    public:
        control_LED(byte pin, byte cmd) //конструктор
        {
            _pin = pin;
            _cmd = cmd;
            pinMode(_pin, OUTPUT);
            digitalWrite(_pin, !(_cmd));
        }

        void switch_LED(byte cmd)
        {
            _cmd = cmd;
            if (_cmd == ON)
            {  digitalWrite(_pin, LOW);    }
            else
            {   digitalWrite(_pin, HIGH);  }
        }
    private:
        byte _pin;
        byte _cmd;   
};

#endif 

