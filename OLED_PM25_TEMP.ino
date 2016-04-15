/*-----Library-----*/
#include <DHT.h>
#include <U8glib.h>
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);

/*-----DHT Setup-----*/
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

/*-----GP2Y-1010 Setup-----*/
int measurePin = 0;
int ledPower = 7;

int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

/*-----Char Setup-----*/
char str[10];

void drawTest()
{
  u8g.setFont(u8g_font_osb18);
  u8g.drawStr( 0, 30, "Loading...");
}

void setup()
{
  Serial.begin(9600);
  delay(500);
  Serial.println("Loading...");
  dht.begin();
  u8g.firstPage();
  u8g.nextPage();  
  pinMode(ledPower,OUTPUT);
  

  do
  {
    drawTest();
  } while(u8g.nextPage());
}

void loop()
{
/*-----GP2Y-1010 Setup-----*/
  digitalWrite(ledPower,LOW); // power on the LED
  delayMicroseconds(samplingTime);
  
  voMeasured = analogRead(measurePin); // read the dust value
  
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH); // turn the LED off
  delayMicroseconds(sleepTime);
  
  // 0 - 5V mapped to 0 - 1023 integer values
  // recover voltage
  calcVoltage = voMeasured * (5.0 / 1024.0);
  
  // linear eqaution taken from http://www.howmuchsnow.com/arduino/airquality/
  // Chris Nafis (c) 2012
  dustDensity = 0.17 * calcVoltage - 0.1;

  Serial.print("Raw Signal Value (0-1023): ");
  Serial.print(voMeasured);
  
  Serial.print(" - Voltage: ");
  Serial.print(calcVoltage);
  
  Serial.print(" - Dust Density: ");
  Serial.print(dustDensity * 1000); // 這裡將數值呈現改成較常用的單位( ug/m3 )
  Serial.println(" ug/m3 ");

  delay(1000);
  //wait a second for measure temp.

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);  //"true" means Fahrenheit

  //Check sensor works!
  if(isnan(h)||isnan(t)||isnan(f))
  {
    Serial.println("Please push the RESET button");
    return;
  }

  //heat index
  float hi = dht.computeHeatIndex(f,h);

  //turn heat index from FahrenHeit to Centigrade
  float hi2 = (hi-32)*5/9;

  /*-----Print in Monitor-----*/
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  
  Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.print(" *C\t ");
  /*
  Serial.print(f);
  Serial.print(" *F\t");
  */
  Serial.print("Heat index: ");
  Serial.print(hi2);
  Serial.println(" *C");

  /*-----print in OLED-----*/

  u8g.firstPage();  
  do {

    u8g.setFont(u8g_font_helvB08);

    u8g.drawStr( 0, 15, "PM2.5:");
    u8g.drawStr( 50, 15, dtostrf(dustDensity * 1000, 5, 2, str));
    u8g.drawStr( 90, 15, "ug/m3");

    u8g.drawStr( 0, 35, "Temperature:");
    u8g.drawStr( 75, 35, dtostrf(t, 5, 2, str));
    u8g.drawStr( 115, 35, "\260C");

    u8g.drawStr( 0, 45, "Humidity:");
    u8g.drawStr( 75, 45, dtostrf(h, 5, 2, str));
    u8g.drawStr( 115, 45, "%");
    
    u8g.drawStr( 0, 60, "Heat index:");
    u8g.drawStr( 75, 60, dtostrf(hi2, 5, 2, str));
    u8g.drawStr( 115, 60, "\260C");

  } while( u8g.nextPage() );
}
