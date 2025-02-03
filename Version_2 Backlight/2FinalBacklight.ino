#include <Wire.h>
#include <BH1750.h>
#include <TFT_eSPI.h>

BH1750 lightMeter(0x23); // I2C address 0x23
TFT_eSPI tft = TFT_eSPI(); // Create TFT object

// Define constants for brightness control
#define MIN_LUX 0
#define MAX_LUX 1000 // Adjust according to your environment
#define MIN_BACKLIGHT 0
#define MAX_BACKLIGHT 255

// Define the PWM pin for backlight control
#define BACKLIGHT_PIN 4 // Change to the appropriate pin for your setup

void setup() {
    Wire.begin();
    lightMeter.begin();
    tft.init();
    tft.setRotation(1); // Adjust rotation if needed
    tft.fillScreen(TFT_BLACK); // Start with a black screen

    // Initialize the PWM pin
    pinMode(BACKLIGHT_PIN, OUTPUT);
}

void loop() {
    // Read ambient light intensity
    float lux = lightMeter.readLightLevel();
    
    // Map lux to backlight brightness
    int backlight = map(lux, MIN_LUX, MAX_LUX, MIN_BACKLIGHT, MAX_BACKLIGHT);
    
    // Constrain the backlight value to the range
    backlight = constrain(backlight, MIN_BACKLIGHT, MAX_BACKLIGHT);

    // Set the backlight brightness using PWM
    analogWrite(BACKLIGHT_PIN, backlight);

    delay(1000); // Adjust delay as needed
}
     
