/*
	Button - a small library for Arduino to handle button debouncing
	
	MIT licensed.
*/

#ifndef PCF_Button_h
#define PCF_Button_h
#include "Arduino.h"
#include <PCF8574.h>

class PCF_Button 
{
	public:
		PCF_Button(PCF8574 &pcf,uint8_t pin, uint16_t debounce_ms = 100);
		void begin();
		bool read();
		bool toggled();
		bool pressed();
		bool released();
		bool has_changed();
		
		const static bool PRESSED = LOW;
		const static bool RELEASED = HIGH;
	
	private:
		PCF8574 _pcf;
		uint8_t  _pin;
		uint16_t _delay;
		bool     _state;
		uint32_t _ignore_until;
		bool     _has_changed;
};

#endif