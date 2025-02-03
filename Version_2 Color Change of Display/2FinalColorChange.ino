#include <Wire.h>
#include <BH1750.h>
#include <TFT_eSPI.h>

BH1750 lightMeter(0x23); // I2C address 0x23
TFT_eSPI tft = TFT_eSPI(); // Create TFT object

#define MIN_LUX 0
#define MAX_LUX 1000
#define MIN_BACKLIGHT 0
#define MAX_BACKLIGHT 255

void setup() {
    Wire.begin();
    lightMeter.begin();
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
}

void loop() {
    float lux = lightMeter.readLightLevel();
    int backlight = map(lux, MIN_LUX, MAX_LUX, MIN_BACKLIGHT, MAX_BACKLIGHT);
    uint8_t alpha = map(backlight, MIN_BACKLIGHT, MAX_BACKLIGHT, 255, 0);
    uint8_t shade = map(lux, MIN_LUX, MAX_LUX, 255, 0);
    uint16_t color = ((alpha >> 3) << 11) | ((shade >> 5) << 5) | (shade & 0x1F);
    tft.fillRect(0, 0, tft.width(), tft.height(), color);
    delay(500); // Adjust delay as needed
}
