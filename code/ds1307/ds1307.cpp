#include"ds1307.h"

#define	ds1307_address 0x68

uint8_t ds1307::bcd2dec(uint8_t num) {
	return ((num / 16 * 10) + (num % 16));
}
uint8_t ds1307::dec2bcd(uint8_t num) {
	return ((num / 10 * 16) + (num % 10));
}
void ds1307::now() {

	Wire.beginTransmission(ds1307_address);
	Wire.write((uint8_t)0x00);
	Wire.endTransmission();
	Wire.requestFrom(ds1307_address, 7);
	second = bcd2dec(Wire.read() & 0x7f);
	minute = bcd2dec(Wire.read());
	hour = bcd2dec(Wire.read() & 0x3f); // chế độ 24h.
	wday = bcd2dec(Wire.read());
	day = bcd2dec(Wire.read());
	month = bcd2dec(Wire.read());
	year = bcd2dec(Wire.read());
	//year += 2000;
}
uint8_t ds1307::get_hour() {
	return hour;
}
uint8_t ds1307::get_minute() {
	return minute;
}
uint8_t ds1307::get_second() {
	return second;
}
uint8_t ds1307::get_day() {
	return day;
}
uint8_t ds1307::get_month() {
	return month;
}
uint8_t ds1307::get_year() {
	return year;
}
uint8_t ds1307::get_wday() {
	return wday;
}
void ds1307::set_time(uint8_t hr, uint8_t min, uint8_t sec, uint8_t wd, uint8_t d, uint8_t mth, uint8_t yr) {
	Wire.beginTransmission(ds1307_address);
	Wire.write(uint8_t(0x00)); // đặt lại pointer
	Wire.write(dec2bcd(sec));
	Wire.write(dec2bcd(min));
	Wire.write(dec2bcd(hr));
	Wire.write(dec2bcd(wd)); // day of week: Sunday = 1, Saturday = 7
	Wire.write(dec2bcd(d));
	Wire.write(dec2bcd(mth));
	Wire.write(dec2bcd(yr));
	Wire.endTransmission();
}
void ds1307::begin() {
	Wire.begin();
}