  /*----------( Declare DHT11 )----------*/
  //the following is setup for sensor
#include "DHT.h"
  //connected digital 2 to DHT11 pin2
#define DHTPIN 2

  //define what sensor you're using
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
  /*
  DHT11-Pin Define & connected
  Pin1 to VCC
  Pin2 to what pin we define, this project is Digital 2 (line 3)
  Pin3 means nothing
  Pin4 to GND
  */

  /*----------( Declare LCD )----------*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
  //Set the pins on the I2C chip used for LCD connections:
  //                  addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

  /*----------( Declare Auduino Setup & Loop )----------*/
void setup()
{
  lcd.begin(16,2);
  lcd.backlight();  //背光，不確定原因，但沒打這行還是會亮
}

void loop()
{
  lcd.setCursor(0, 0); //將游標設定在第一行第一個位置(column 0, line 1)  


  //wait a few seconds for measurements.
  delay(500);

  //humidity, temperature,
  float h = dht.readHumidity();
  float t = dht.readTemperature();


  //check if any wrong or fail.
  if(isnan(h) || isnan(t))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(t, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  
  lcd.print("Humidity: ");       
  lcd.print(h);       
  lcd.print("%");       
  lcd.setCursor(0,1); //換行
  lcd.print("Temp: ");       
  lcd.print(t);       
  lcd.print("*C   ");
  
  
}
