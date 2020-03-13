#pragma once
#include <Wire.h>
class ds1307
{
public:
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint8_t wday;
	uint8_t month;
	uint8_t year;
	void begin();
	void set_time(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
	void now();
	uint8_t get_hour();
	uint8_t get_minute();
	uint8_t get_second();
	uint8_t get_day();
	uint8_t get_wday();
	uint8_t get_month();
	uint8_t get_year();

private:
	
	uint8_t bcd2dec(uint8_t);
	uint8_t dec2bcd(uint8_t);
};


