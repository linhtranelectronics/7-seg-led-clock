  #include<ds1307.h>



  ds1307 ds1307;

  // the setup function runs once when you press reset or power the board
  void setup() {
    Serial.begin(115200);
    ds1307.begin();
  }

  // the loop function runs over and over again until power down or reset
  void loop() {
    ds1307.now();
    Serial.print(ds1307.get_hour());
    Serial.print("\t");
    Serial.print(ds1307.get_minute());
    Serial.print("\t");
    Serial.println(ds1307.get_second());
    delay(100);
  }