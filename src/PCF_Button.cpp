/*
	PCF_Button - a small library for Arduino to handle PCF_Button debouncing
	
	MIT licensed.
*/

#include "PCF_Button.h"
#include <Arduino.h>
#include <PCF8574.h>

PCF_Button::PCF_Button(PCF8574 &pcf, byte pin, uint16_t debounce_ms)
:  _pcf(pcf)
,  _pin(pin)
,  _delay(debounce_ms)
,  _state(HIGH)
,  _ignore_until(0)
,  _has_changed(false)
{
}

void PCF_Button::begin()
{
	// pinMode(_pin, INPUT_PULLUP);
	// _pcf.write(pin, 1); //Maybe?
}

// 
// public methods
// 

bool PCF_Button::read()
{
	// ignore pin changes until after this delay time
	if (_ignore_until > millis())
	{
		// ignore any changes during this period
	}
	
	// pin has changed 
	else if (_pcf.read(_pin) != _state)
	{
		_ignore_until = millis() + _delay;
		_state = !_state;
		_has_changed = true;
	}
	
	return _state;
}

// has the PCF_Button been toggled from on -> off, or vice versa
bool PCF_Button::toggled()
{
	read();
	return has_changed();
}

// mostly internal, tells you if a PCF_Button has changed after calling the read() function
bool PCF_Button::has_changed()
{
	if (_has_changed)
	{
		_has_changed = false;
		return true;
	}
	return false;
}

// has the PCF_Button gone from off -> on
bool PCF_Button::pressed()
{
	return (read() == PRESSED && has_changed());
}

// has the PCF_Button gone from on -> off
bool PCF_Button::released()
{
	return (read() == RELEASED && has_changed());
}