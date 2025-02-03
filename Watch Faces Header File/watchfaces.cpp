#include <SPI.h>
#include <TFT_eSPI.h> 
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "fonts.h"
#include "watchfaces.h"

#define TFT_GREY 0x5AEB
#define TFT_SKYBLUE 0x067D
#define color1 TFT_WHITE
#define color2 0x8410
#define color3 0x5ACB
#define color4 0x15B3
#define color5 0x00A3
#define background 0xB635

TFT_eSPI tft = TFT_eSPI();       
TFT_eSprite img = TFT_eSprite(&tft);

float sx = 0, sy = 1, mx = 1, my = 0, hx = -1, hy = 0, sdeg = 0, mdeg = 0, hdeg = 0;
uint16_t osx = 120, osy = 140, omx = 120, omy = 140, ohx = 120, ohy = 140, x0 = 0, x1 = 0, yy0 = 0, yy1 = 0;
uint32_t targetTime = 0;
unsigned long lastfacechange = 0;
bool initial = 1;
float VALUE;
float lastValue = 0;
int lastsec = 0;
double rad = 0.01745;
float x[360], y[360];
int watchface = 0;
bool facechange = false;
float px[360], py[360], lx[360], ly[360];
int r = 104, ssx = 120, ssy = 140;

String cc[12] = {"45", "40", "35", "30", "25", "20", "15", "10", "5", "0", "55", "50"};
String days[] = {"SUNDAY", "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY"};
String days1[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

int start[12], startP[60];

int angle = 0;
bool onOff = 0;
bool debounce = 0;
String h, m, s, d1, d2, m1, m2;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800); // UTC offset for IST is 5 hours and 30 minutes

const int buttonPin = 36;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

//wfclass::wfclass(){}

void wfclass::setup(void) {
  WiFi.begin("ANKIT", "ankit2001"); // Replace with your Wi-Fi credentials
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  timeClient.begin();
  timeClient.setTimeOffset(19800);
  timeClient.update();
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonPressed, FALLING);
  tft.init();
  tft.setRotation(0);
  tft.setSwapBytes(true); 
  tft.fillScreen(TFT_BLACK);
  img.setColorDepth(8);
  img.setSwapBytes(true);
  img.createSprite(240, 280);
  img.setTextDatum(4);
  targetTime = millis() + 1000;
  facechange = true;

  updateNTPTime();
  updateClock();
}

void wfclass::loop() {
  int buttonState = digitalRead(buttonPin);
  if (buttonState == LOW && lastButtonState == HIGH && millis() - lastDebounceTime > debounceDelay) {
    watchface++;
    if (watchface > 3) {
      watchface = 0;
    }
    facechange = true;
    lastDebounceTime = millis();
  }
  lastButtonState = buttonState;

  // Update time every second
  if (millis() >= targetTime) {
    targetTime = millis() + 1000;
    updateNTPTime();
    updateClock();
  }
  if (timeClient.update()) {
    updateClock();
  }
}

void wfclass::updateNTPTime() {
  // If WiFi is connected, update time from NTP server
  if (WiFi.status() == WL_CONNECTED) {
    timeClient.update();
  }
}

void wfclass::updateClock() {
  unsigned long epochTime = timeClient.getEpochTime();
  time_t rawTime = (time_t)epochTime;
  struct tm *currentTime = localtime(&rawTime);

  int hh = currentTime->tm_hour;
  int mm = currentTime->tm_min;
  int ss = currentTime->tm_sec;
  int dayOfMonth = currentTime->tm_mday;
  int dayOfWeek = currentTime->tm_wday;
  
switch (watchface) {
    case 0:
      drawAnalogClock(hh, mm, ss, dayOfWeek);
      break;
    case 1:
      drawDigitalClockWithDate(hh, mm, ss, dayOfMonth, dayOfWeek);
      break;
    case 2:
      drawWatchFace(hh, mm, ss, dayOfMonth, dayOfWeek);
      break;
    default:
      drawAnalogClock(hh, mm, ss, dayOfWeek);
  }
}
int lastAngle = 0;
float circle = 100;
bool dir = 0;
int rAngle = 359;

void wfclass::drawAnalogClock(int hh, int mm, int ss, int dayOfWeek) {
  if (ss != lastsec)
  {
      img.setTextColor(TFT_WHITE, TFT_BLACK);
      img.fillCircle(120, 140, 118, TFT_GREEN);
      img.fillCircle(120, 140, 110, TFT_BLACK);
      for (int i = 0; i < 360; i += 30) {
        sx = cos((i - 90) * 0.0174532925);
        sy = sin((i - 90) * 0.0174532925);
        x0 = sx * 114 + 120;
        yy0 = sy * 114 + 140;
        x1 = sx * 100 + 120;
        yy1 = sy * 100 + 140;
        img.drawLine(x0, yy0, x1, yy1, TFT_GREEN);
      }
      for (int i = 0; i < 360; i += 6) {
        sx = cos((i - 90) * 0.0174532925);
        sy = sin((i - 90) * 0.0174532925);
        x0 = sx * 102 + 120;
        yy0 = sy * 102 + 140;
        img.drawPixel(x0, yy0, TFT_WHITE);
        
        if (i == 0 || i == 180) img.fillCircle(x0, yy0, 2, TFT_WHITE);
        if (i == 90 || i == 270) img.fillCircle(x0, yy0, 2, TFT_WHITE);
      }
      img.fillCircle(120, 141, 3, TFT_WHITE);
      sdeg = ss * 6;                
      mdeg = mm * 6 + sdeg * 0.01666667; 
      hdeg = hh * 30 + mdeg * 0.0833333; 
      hx = cos((hdeg - 90) * 0.0174532925);
      hy = sin((hdeg - 90) * 0.0174532925);
      mx = cos((mdeg - 90) * 0.0174532925);
      my = sin((mdeg - 90) * 0.0174532925);
      sx = cos((sdeg - 90) * 0.0174532925);
      sy = sin((sdeg - 90) * 0.0174532925);
      if (ss == 0 || initial) {
        initial = 0;
        
        img.drawLine(ohx, ohy, 120, 141, TFT_BLACK);
        ohx = hx * 62 + 121;
        ohy = hy * 62 + 141;
        img.drawLine(omx, omy, 120, 141, TFT_BLACK);
        omx = mx * 84 + 120;
        omy = my * 84 + 141;
      }
      img.drawLine(osx, osy, 120, 141, TFT_BLACK);
      osx = sx * 90 + 121;
      osy = sy * 90 + 141;
      img.drawLine(osx, osy, 120, 141, TFT_RED);
      img.drawLine(ohx, ohy, 120, 141, TFT_WHITE);
      img.drawLine(omx, omy, 120, 141, TFT_WHITE);
      img.drawLine(osx, osy, 120, 141, TFT_RED);
      img.fillCircle(120, 141, 3, TFT_RED);
      img.pushSprite(0, 0);
    }
    lastsec = ss;
  }


void wfclass::drawDigitalClockWithDate(int hh, int mm, int ss, int dayOfMonth, int dayOfWeek) {
  {
    int b = 0;
    int b2 = 0;
    for (int i = 0; i < 360; i++)
    {
      x[i] = (r * cos(rad * i)) + ssx;
      y[i] = (r * sin(rad * i)) + ssy;
      px[i] = ((r - 16) * cos(rad * i)) + ssx;
      py[i] = ((r - 16) * sin(rad * i)) + ssy;
      lx[i] = ((r - 26) * cos(rad * i)) + ssx;
      ly[i] = ((r - 26) * sin(rad * i)) + ssy;
      if (i % 30 == 0) {
        start[b] = i;
        b++;
      }
      if (i % 6 == 0) {
        startP[b2] = i;
        b2++;
      }
    }
     rAngle = rAngle - 2;
    angle = ss * 6;
    s = String(ss);
    m = String(mm);
    h = String(hh);
    if (m.toInt() < 10)
      m = "0" + m;
    if (h.toInt() < 10)
      h = "0" + h;
    if (s.toInt() < 10)
      s = "0" + s;
    if (dayOfMonth > 10)
    {
      d1 = dayOfMonth / 10;
      d2 = dayOfMonth % 10;
    }
    else
    {
      d1 = "0";
      d2 = String(dayOfMonth);
    }
    if (dayOfMonth > 10)
    {
       m1 = "0";
       m2 = "5";
    }
    else
    {
      m1 = "0";
      m2 = "5";
    }
    if (angle >= 360)
      angle = 0;
    if (rAngle <= 0)
      rAngle = 359;
    if (dir == 0)
      circle = circle + 0.5;
    else
      circle = circle - 0.5;
    if (circle > 140)
      dir = !dir;
    if (circle < 100)
      dir = !dir;
    if (angle > -1)
    {
      lastAngle = angle;
      VALUE = ((angle - 270) / 3.60) * -1;  
      if (VALUE < 0)
        VALUE = VALUE + 100;
      img.fillSprite(TFT_BLACK);
      img.fillCircle(ssx, ssy, 124, color5);
      img.setTextColor(TFT_WHITE, color5);
      img.drawString(days[dayOfWeek], circle, 140, 2);

      for (int i = 0; i < 12; i++)
        if (start[i] + angle < 360) {
          img.drawString(cc[i], x[start[i] + angle], y[start[i] + angle], 2);
          img.drawLine(px[start[i] + angle], py[start[i] + angle], lx[start[i] + angle], ly[start[i] + angle], color1);
        }
        else
        {
          img.drawString(cc[i], x[(start[i] + angle) - 360], y[(start[i] + angle) - 360], 2);
          img.drawLine(px[(start[i] + angle) - 360], py[(start[i] + angle) - 360], lx[(start[i] + angle) - 360], ly[(start[i] + angle) - 360], color1);
        }
      img.setFreeFont(&DSEG7_Modern_Bold_20);
      img.drawString(s, ssx, ssy - 36);
      img.setFreeFont(&DSEG7_Classic_Regular_28);
      img.drawString(h + ":" + m, ssx, ssy + 28);
      img.setTextFont(0);
      img.fillRect(70, 86, 12, 20, color3);
      img.fillRect(84, 86, 12, 20, color3);
      img.fillRect(150, 86, 12, 20, color3);
      img.fillRect(164, 86, 12, 20, color3);
      img.setTextColor(0x35D7, TFT_BLACK);
      img.drawString("MONTH", 84, 78);
      img.drawString("DAY", 162, 78);
      img.setTextColor(TFT_SKYBLUE, TFT_BLACK);
      img.drawString("Swajal K Jha", 120, 194);
      img.drawString("*****", 120, 124);
      img.setTextColor(TFT_WHITE, color3);
      img.drawString(m1, 77, 96, 2);
      img.drawString(m2, 91, 96, 2);
      img.drawString(d1, 157, 96, 2);
      img.drawString(d2, 171, 96, 2);


      for (int i = 0; i < 60; i++)
        if (startP[i] + angle < 360)
          img.fillCircle(px[startP[i] + angle], py[startP[i] + angle], 1, color1);
        else
          img.fillCircle(px[(startP[i] + angle) - 360], py[(startP[i] + angle) - 360], 1, color1);
      img.fillTriangle(ssx - 1, ssy - 70, ssx - 5, ssy - 56, ssx + 4, ssy - 56, TFT_ORANGE);
      img.pushSprite(0, 0);
    }
  }
  lastsec = ss;
}

void wfclass::drawWatchFace(int hh, int mm, int ss,int dayOfMonth, int dayOfWeek) {
  if (ss != lastsec)
  {
    tft.fillScreen(TFT_BLACK);
      {
      String med;
      if (ss % 2) {
        med = ":";
      }
      else
      {
        med = " " ;
      }
      tft.setFreeFont(&DSEG7_Classic_Bold_30);
      if (hh >= 10 && mm >= 10)
      {
        tft.drawString(String(hh) + ":" + String(mm), 40, 130);
      }
      else if (hh < 10 && mm > 10)
      { 
        tft.drawString("0" + String(hh) + ":" + String(mm), 40, 130);
      }
      else if (hh > 10 && mm < 10)
      {
        tft.drawString(String(hh) + ":0" + String(mm), 40, 130);
      }
      else
      {
        tft.drawString("0" + String(hh) + ":0" + String(mm), 40, 145);
      }
      tft.setFreeFont(&DSEG7_Classic_Bold_20);
      if (ss < 10)
      {
        tft.drawString(":0" + String(ss), 150, 140);
      }
      else
      {
        tft.drawString(":" + String(ss), 150, 140);
      }
      tft.setFreeFont(&DSEG14_Classic_Bold_18);
      tft.drawString(days1[dayOfWeek], 90, 105);
      tft.drawString(String(dayOfMonth), 155, 105);
    }
  }
  }

void wfclass::buttonPressed() {
  // Empty function to handle button press interrupt
}

wfclass WatchFaces = wfclass();
