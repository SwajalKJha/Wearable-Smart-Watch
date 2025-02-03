#include <SPI.h>
#include <TFT_eSPI.h> 
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "watchfaces.h"
#include "fonts.h"
TFT_eSPI tft = TFT_eSPI();       
TFT_eSprite img = TFT_eSprite(&tft);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800); // UTC offset for IST is 5 hours and 30 minutes
void setup(void) {
  watchfaces.setup(void);
}
void loop() {
  watchfaces.loop();
}
void watchfaces.updateNTPTime();
void watchfaces.updateClock();
void watchfaces.drawAnalogClock(int hh, int mm, int ss, int dayOfWeek);
void watchfaces.drawDigitalClockWithDate(int hh, int mm, int ss, int dayOfMonth, int dayOfWeek);
void watchfaces.drawWatchFace(int hh, int mm, int ss,int dayOfMonth, int dayOfWeek);
void watchfaces.buttonPressed();
