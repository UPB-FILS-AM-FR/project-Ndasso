#include "DHT.h"
#define DHTPIN A0     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

#include <LCD_I2C.h>
LCD_I2C lcd(0x27, 16, 2); // Default address of most PCF8574 modules, change according
byte celsius[8] = {  0x00, 0x0E,  0x0A,  0x0E,  0x00,  0x00,  0x00, 0x00 };

#include <Bonezegei_DS1307.h>
Bonezegei_DS1307 rtc(0x68);

#include <Wire.h>

void setup() {
  dht.begin();

  rtc.begin();

  lcd.begin(false); // If you are using more I2C devices using the Wire library use lcd.begin(false)
  lcd.backlight();  //back light on
  lcd.createChar(1, celsius); 
}

void loop() { 
  int humidity = dht.readHumidity();
  float temp = dht.readTemperature();
  if (isnan(humidity) || isnan(temp)) { // Is Not A Number
    humidity = 51;
    temp = 23.4;
  }
  char sHum[16] = "";
  char sTemp[16] = "";
  char fTemp[16];
  dtostrf(temp,4,1,fTemp);
  sprintf(sHum, "%2d%%Hum", humidity);
  sprintf(sTemp, "%s%cC", fTemp, 1 );  //byte(1)   0x01  ascii 1

  char sTime[16] = "";
  char sDate[16] = "";
  if (rtc.getTime()) {
    sprintf(sTime, "%02d:%02d:%02d  ", rtc.getHour(), rtc.getMinute(), rtc.getSeconds());
    sprintf(sDate, "%02d/%02d/%02d  ", rtc.getDate(), rtc.getMonth(), rtc.getYear());
    }
    

  lcd.clear();
  lcd.setCursor(0, 0); //  setting the cursor in the desired position.
  lcd.print(sDate);
 
  lcd.print(sHum);
  lcd.setCursor(0, 1); //  setting the cursor in the desired position.
  lcd.print(sTime);
  lcd.print(sTemp); 

  delay(1000);
}
