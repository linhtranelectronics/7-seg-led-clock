/*
 Name:    _7seg_clock.ino
 Created: 3/10/2020 2:54:31 PM
 Author:  linh
*/

// the setup function runs once when you press reset or power the board
#include <ds1307.h>
#include <JC_Button.h>

#define blinkPin 9
#define DS_pin 10
#define ST_pin 11
#define SH_pin 12
#define Led1 13
#define Led2 8
#define Led3 7
#define Led4 6
#define Led5 5
#define Led6 4
#define Led7 3
#define Led8 2
#define buttonSet_pin 15
#define buttonUp_pin 14
#define buttonDown_pin 16

uint32_t nowMili, lastMiliScan, lastMiliBLink, lastMiliGetTime, lastMiliLongpress;
uint8_t numLed7seg;
uint8_t hour, minute, second, wday, day, month, year;
bool hide = false; // blink led che do cai dat
bool mode = 0; // mode = 0 (hien thi gio), mode = 1 cai dat thoi gian
uint8_t menu = 0;
uint16_t longpress = 1000;
const int Seg[12] = {
  0b11101110,//0
  0b10001000,//1
  0b11010110,//2
  0b11011100,//3
  0b10111000,//4
  0b01111100,//5
  0b01111110,//6
  0b11001000,//7
  0b11111110,//8
  0b11111100,//9
  0b00010000,// -
  0b00000000 // off all
};
ds1307 ds1307;
Button buttonUp(buttonUp_pin);
Button buttonDown(buttonDown_pin);
Button buttonSet(buttonSet_pin);
void scanCathode(uint8_t byte, uint8_t byte2);
void scanAnode(uint8_t numLed);
void scan7seg(uint16_t ms, uint8_t num1, uint8_t num2, uint8_t num3, uint8_t num4, uint8_t num5, uint8_t num6,
  uint8_t num7, uint8_t num8, uint8_t num9, uint8_t num10, uint8_t num11, uint8_t num12, uint8_t num13, uint8_t num14);
void setUpTime();
void gettime();
void readButton(uint8_t &val, uint8_t min, uint8_t max);
void setup() {
  for (uint8_t i = 2; i < 14; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  pinMode(buttonDown_pin, INPUT_PULLUP);
  pinMode(buttonSet_pin, INPUT_PULLUP);
  pinMode(buttonUp_pin, INPUT_PULLUP);
  scan7seg(1, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11);
  ds1307.begin();
  Serial.begin(115200);
}
// the loop function runs over and over again until power down or reset
void loop() {
  buttonDown.read();
  buttonSet.read();
  buttonUp.read();
  if (mode == 0) {
    gettime();
    scan7seg(1, day / 10, day % 10, month / 10, month % 10, 2, 0, year / 10, year % 10,
      hour / 10, hour % 10, minute / 10, minute % 10, second / 10, second % 10);
  }
  if (mode == 1) {
    setUpTime();
  }
  if (buttonSet.pressedFor(longpress) && mode == 0) {
    mode = 1;
    menu = 0;
    scan7seg(1, 11, 11, 11, 11, 11, 11, 11, 11,
      11, 11, 11, 11, 11, 11);
  }
  if (buttonSet.wasReleased() && mode == 1) {
    menu++;
    if (menu == 7) {
      ds1307.set_time(hour, minute, second, 0, day, month, year);
      mode = 0;
    }
  }
}

void scanCathode(uint8_t byte1, uint8_t byte2) {
  digitalWrite(ST_pin, LOW);
  shiftOut(DS_pin, SH_pin, LSBFIRST, Seg[byte1]);
  shiftOut(DS_pin, SH_pin, LSBFIRST, Seg[byte2]);
  digitalWrite(ST_pin, HIGH);
}
void scanAnode(uint8_t numLed) {
  switch (numLed) {
  case 1:
    digitalWrite(Led8, HIGH);
    digitalWrite(Led1, LOW);
    break;
  case 2:

    digitalWrite(Led1, HIGH);
    digitalWrite(Led2, LOW);
    break;
  case 3:

    digitalWrite(Led2, HIGH);
    digitalWrite(Led3, LOW);
    break;
  case 4:

    digitalWrite(Led3, HIGH);
    digitalWrite(Led4, LOW);
    break;
  case 5:

    digitalWrite(Led4, HIGH);
    digitalWrite(Led5, LOW);
    break;
  case 6:

    digitalWrite(Led5, HIGH);
    digitalWrite(Led6, LOW);
    break;
  case 7:

    digitalWrite(Led6, HIGH);
    digitalWrite(Led7, LOW);
    break;
  case 8:

    digitalWrite(Led7, HIGH);
    digitalWrite(Led8, LOW);
    break;

  default:
    break;
  }
}
void scan7seg(uint16_t ms, uint8_t num1, uint8_t num2, uint8_t num3, uint8_t num4, uint8_t num5, uint8_t num6,
  uint8_t num7, uint8_t num8, uint8_t num9, uint8_t num10, uint8_t num11, uint8_t num12, uint8_t num13, uint8_t num14) {
  bool scan = false;
  nowMili = millis();
  if ((uint32_t)(nowMili - lastMiliScan) >= ms) {
    lastMiliScan = nowMili;
    scan = true;
    numLed7seg++;
    if (numLed7seg == 8) {
      numLed7seg = 0;
    }
  }
  if (scan) {
    switch (numLed7seg) {
    case 0:
      scanCathode(num1, num9);
      scanAnode(1);
      break;
    case 1:
      scanCathode(num2, num10);
      scanAnode(2);
      break;
    case 2:
      scanCathode(num3, num11);
      scanAnode(3);
      break;
    case 3:
      scanCathode(num4, num12);
      scanAnode(4);
      break;
    case 4:
      scanCathode(num5, num13);
      scanAnode(5);
      break;
    case 5:
      scanCathode(num6, num14);
      scanAnode(6);
      break;
    case 6:
      scanCathode(num7, 0);
      scanAnode(7);
      break;
    case 7:
      scanCathode(num8, 0);
      scanAnode(8);
      break;
    default:
      break;
    }
  }
}

void setUpTime() {
  //nowMili = millis();
  if ((uint32_t)(nowMili - lastMiliBLink) >= 200) {
    hide = !hide; // nhap nhay so dang cai dat
    lastMiliBLink = nowMili;
  }
  switch (menu) {
  case 1: // cai dat ngay
    if (hide) {
      scan7seg(1, 11, 11, 10, 10, 10, 10, 10, 10,
        10, 10, 10, 10, 10, 10);
    } else {
      scan7seg(1, day / 10, day % 10, 10, 10, 10, 10, 10, 10,
        10, 10, 10, 10, 10, 10);
    }
    readButton(day, 1, 31);
    break;
  case 2: // cai dat thang
    if (hide) {
      scan7seg(1, 10, 10, 11, 11, 10, 10, 10, 10,
        10, 10, 10, 10, 10, 10);
    } else {
      scan7seg(1, 10, 10, month / 10, month % 10, 10, 10, 10, 10,
        10, 10, 10, 10, 10, 10);
    }
    readButton(month, 1, 12);
    break;
  case 3: // cai dat nam
    if (hide) {
      scan7seg(1, 10, 10, 10, 10, 11, 11, 11, 11,
        10, 10, 10, 10, 10, 10);
    } else {
      scan7seg(1, 10, 10, 10, 10, 2, 0, year / 10, year % 10,
        10, 10, 10, 10, 10, 10);
    }
    readButton(year, 20, 60);
    break;
  case 4: // cai dat gio
    if (hide) {
      scan7seg(1, 10, 10, 10, 10, 10, 10, 10, 10,
        11, 11, 10, 10, 10, 10);
    } else {
      scan7seg(1, 10, 10, 10, 10, 10, 10, 10, 10,
        hour / 10, hour % 10, 10, 10, 10, 10);
    }
    readButton(hour, 0, 23);
    break;
  case 5: // cai dat phut
    if (hide) {
      scan7seg(1, 10, 10, 10, 10, 10, 10, 10, 10,
        10, 10, 11, 11, 10, 10);
    } else {
      scan7seg(1, 10, 10, 10, 10, 10, 10, 10, 10,
        10, 10, minute / 10, minute % 10, 10, 10);
    }
    readButton(minute, 0, 59);
    break;
  case 6: // cai dat giay
    if (hide) {
      scan7seg(1, 10, 10, 10, 10, 10, 10, 10, 10,
        10, 10, 10, 10, 11, 11);
    } else {
      scan7seg(1, 10, 10, 10, 10, 10, 10, 10, 10,
        10, 10, 10, 10, second / 10, second % 10);
    }
    readButton(second, 0, 59);
    break;

  default:
    break;
  }
}
void gettime() {
  nowMili - millis();
  if ((uint32_t)(nowMili - lastMiliGetTime) >= 500) {
    digitalWrite(blinkPin, !digitalRead(blinkPin));
    lastMiliGetTime = nowMili;
    ds1307.now();
    hour = ds1307.get_hour();
    minute = ds1307.get_minute();
    second = ds1307.get_second();
    day = ds1307.get_day();
    month = ds1307.get_month();
    year = ds1307.get_year();
  }
}
void readButton(uint8_t &val, uint8_t min, uint8_t max) {
  int8_t value = val;
  if (buttonDown.wasReleased()) {
    value--;
    if (value < min) {
      value = max;
    }
  }
  if (buttonUp.wasReleased()) {
    value++;
    if (value > max) {
      value = min;
    }
  }
  if (buttonUp.pressedFor(longpress)) {
    if ((uint32_t)(nowMili - lastMiliLongpress) >= 100) {
      lastMiliLongpress = nowMili;
      value++;
      if (value > max) {
        value = min;
      }
    }
  }
  if (buttonDown.pressedFor(longpress)) {
    if ((uint32_t)(nowMili - lastMiliLongpress) >= 100) {
      lastMiliLongpress = nowMili;
      value--;
      if (value < min) {
        value = max;
      }
    }
  }
  val = value;
}
