#include "Adafruit_GFX.h" 
#include <MCUFRIEND_kbv.h>
#include "RTClib.h"


MCUFRIEND_kbv tft;
DateTime now;
void readVoltage(void);
float getVoltageAverage();
void showdate(void);
void showtime(void);
void showday(void);


#include "Fonts/FreeSans9pt7b.h"
#include "Fonts/FreeSans12pt7b.h"
#include "Fonts/FreeSerif12pt7b.h"
#include "Fonts/FreeMonoBold12pt7b.h"
#include "Fonts/FreeMonoBold18pt7b.h"
#include "Fonts/FreeMonoBold24pt7b.h"
#include "FreeDefaultFonts.h"


#define BLACK 0x0000
#define NAVY 0x000F
#define DARKGREEN 0x03E0
#define DARKCYAN 0x03EF
#define MAROON 0x7800
#define PURPLE 0x780F
#define OLIVE 0x7BE0
#define LIGHTGREY 0xC618
#define DARKGREY 0x7BEF
#define BLUE 0x001F
#define GREEN 0x07E0
#define CYAN 0x07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define ORANGE 0xFD20
#define GREENYELLOW 0xAFE5
#define PINK 0xF81F
RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
#include <SimpleDHT.h>
#include "DHT.h"
#define DHTPIN 28 
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);

#include <Wire.h>
int pert;
int smokeA0=A15;
float sensorValue;
const float arduinoVCC = 5.01;//Your Arduino voltage
unsigned long ValueR1 = 4930;
unsigned long ValueR2 = 99800;
double Voltage_Source = 72;
const int alanogPin = A14;//the pin connecting the voltage. 
const int inputResolution =1023;//works with most Arduino boards
const float average_of = 500;//Average of 500 readings
float v;
float voltage;
void showmsgXY(int x, int y, int sz, const GFXfont *f, const char *msg)
{
  int16_t x1, y1;
  uint16_t wid, ht;
  tft.setFont(f);
  tft.setCursor(x, y);
  tft.setTextSize(sz);
  tft.println(msg);
}
uint8_t r = 255, g = 255, b = 255;
uint16_t color;
void setup()
{
  tft.reset();
  Serial.begin(9600);
   pinMode(smokeA0,INPUT);
   dht.begin();
  #ifndef ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
#endif

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  uint16_t ID = tft.readID();
  tft.begin(ID);
  tft.setRotation(1);
  tft.fillScreen(BLACK);
  tft.setTextSize(4);
  tft.setTextColor(WHITE);
  tft.println(F("TEAM FALCON CRUISERS"));
  tft.setTextSize(2);
  tft.println(F("        *****THE FASTEST*****"));
  
  
 
}


void loop(){
  int analogSensor = analogRead(smokeA0);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f) || isnan(analogSensor)){
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
   now = rtc.now();
showtime();
showday();
showdate(); 
float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);
tft.fillScreen(0xFFFF);
  color = tft.color565(0, 0, 0);
  tft.setTextColor(color);
  showmsgXY(20, 30, 1, &FreeMonoBold18pt7b,"    FALCON CRUISERS"); 
  showmsgXY(0, 40, 1.8, &FreeSerif12pt7b,"________________________________________"); 
  tft.print(" VP:");
  tft.print(analogSensor); //delay(100);
  tft.println(F("Rh"));
  tft.println(" ");
  tft.print((" H:"));
  tft.print(h);//delay(100);
  tft.println(F("%"));
  tft.println(" ");
  tft.print(F(" T:"));
  tft.print(t);//delay(100);
  tft.println(F("˚C"));
  tft.println(" ");
  tft.print(F("H.I:")); //delay(100);
  tft.print(hic);
  tft.print(F("C"));//delay(100);
  

  showmsgXY(0, 280, 1, &FreeSerif12pt7b ,"________________________________________"); 
   tft.print(now.day());//delay(100);
  tft.print('/');
  tft.print(now.month());//delay(100);
  tft.print('/');
  tft.print(now.year());//delay(100);
  tft.print(daysOfTheWeek[now.dayOfTheWeek()]);//delay(100);
  tft.print("                     Time:");
  tft.print(now.hour());//delay(100);
  tft.print(':');
 tft.print(now.minute());//delay(100);
  tft.print(':');
 tft.print(now.second());//delay(100);
  tft.print("    ");
  tft.drawFastVLine(120,45,240,RED);
  tft.drawFastHLine(122,80,370,RED);
  tft.drawFastVLine(375,80,200,RED);
  showmsgXY(300, 100, 2, &FreeMonoBold24pt7b,"");
  int Speed = 25;
  if(Speed<=45)
  {
    tft.print("       1");
  }
  else if(Speed>45 || Speed<=60)
  {
    tft.print("       2");
    }
  else
  {
    tft.print("       3");
  }
  
  int val = true;
  tft.drawFastHLine(380,250,110,OLIVE);
  showmsgXY(380, 250, 1, &FreeSerif12pt7b,"");
  if (val) {
    tft.println("                                                               Shift to-0");
  } else {
    tft.println("                                                               Shift to-1");
  }
  Serial.println();
readVoltage();
v=getVoltageAverage();
  tft.setCursor(125,70);
  tft.print("Vin: ");
  //Serial.print(voltage);
  //Serial.print("V Avg: ");
  tft.print(v);
  tft.print("V");
  tft.print("  ");
  if (v>=56 && v<=90)
  {
    pert=((v-56)*100)/(90-56);
    tft.print(pert);
    tft.print("%");
  }
  if (v<56)
  {
    tft.print("Battery LOW");
  }
  
 
  showmsgXY(125, 70, 1, &FreeSerif12pt7b,"");
  
}

  

void showtime(){
  Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
}
void showday(){
  Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
}
void showdate(){
   Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.println();
    delay(1000);
}
void readVoltage(){
    //Robojax.com ARDVC-01 Measure any voltage with Arduino
    //This code is explained at my Arduino Course at udemy.com http://robojax.com/L/?id=62
    int A0Value = analogRead(alanogPin);
    float voltage_sensed = A0Value * (arduinoVCC / (float)inputResolution); 
//  Serial.print("voltage_sensed:");
//  Serial.print(voltage_sensed);       
  voltage = voltage_sensed * ( 1 + ( (float) ValueR2 /  (float) ValueR1) );
}
float getVoltageAverage(){
  //Robojax.com ARDVC-01 Measure any voltage with Arduino
    //This code is explained at my Arduino Course at udemy.com http://robojax.com/L/?id=62  
    float voltage_temp_average=0;
    for(int i=0; i < average_of; i++)
    {
       readVoltage();
       voltage_temp_average +=voltage;
    }
      
  return voltage_temp_average / average_of;
}
