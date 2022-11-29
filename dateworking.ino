#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <Adafruit_GFX.h>
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define GREY 0x8410
#define RED 0xF800
#include "RTClib.h"
DateTime now;
MCUFRIEND_kbv tft;
RTC_DS3231 rtc;
int activate;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
void setup()
{
  Serial.begin(9600);
  tft.reset();
  tft.begin(tft.readID());
  tft.setRotation(1);
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

  // When time needs to be re-set on a previously configured device, the
  // following line sets the RTC to the date & time this sketch was compiled
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
}
void loop()
{
  now = rtc.now();
  tft.fillRect(170, 50, 50, 50, BLACK);


  // update
  if (now.second() <= 1) {
    tft.fillScreen(BLACK);
  }
  // Day
  tft.setCursor(50, 180);
  tft.setTextSize(4);
  tft.setTextColor(CYAN);
  tft.print(daysOfTheWeek[now.dayOfTheWeek()]);

  // Heading
  tft.setCursor(50, 10);
  tft.setTextColor(MAGENTA);
  tft.setTextSize(3);
  tft.print("Digital Clock");

  //time
  tft.setCursor(30, 60);
  tft.setTextSize(4);
  tft.setTextColor(GREEN);

  if (now.hour() >= 12) {
    activate = 1;


    if (now.hour() <= 9) {
      tft.print("0");
      tft.print(now.hour());
      tft.print(":");
    }
    else {
      tft.print(now.hour());
      tft.print(":");
    }
  }
  else {
    activate = 0;

  }
  if (now.hour() <= 9) {
    tft.print("0");
    tft.print(now.hour());
    tft.print(":");
  }
 

if (now.minute() <= 9) {
  tft.print("0");
  tft.print(now.minute());
  tft.print(":");
}
else {
  tft.print(now.minute());
  tft.print(":");
}

if (now.second() <= 9) {
  tft.print("0");
  tft.print(now.second());
  tft.print(" ");
}
else {
  tft.print(now.second());
  tft.print(" ");
}



//Date
tft.setCursor(30, 120);
tft.setTextColor(YELLOW);
if (now.day() <= 9) {
  tft.print("0");
  tft.print(now.day());
  tft.print(" ");
}
else {
  tft.print(now.day());
  tft.print(" ");
}

switch (now.month())
{
case 1:
  tft.print("JAN");
  break;
case 2:
  tft.print("FEB");
  break;
case 3:
  tft.print("MAR");
  break;
case 4:
  tft.print("APR");
  break;
case 5:
  tft.print("MAY");
  break;
case 6:
  tft.print("JUN");
  break;
case 7:
  tft.print("JUL");
  break;
case 8:
  tft.print("AUG");
  break;
case 9:
  tft.print("SEP");
  break;
case 10:
  tft.print("OCT");
  break;
case 11:
  tft.print("NOV");
  break;
case 12:
  tft.print("DEC");
  break;
}

tft.setCursor(200, 120);
tft.print(now.year());
delay(1000);
}
