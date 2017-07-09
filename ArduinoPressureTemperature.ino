// Adafruit OLED FeatherWing Battery Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.
//
// Battery Example (by Todd Treece) was used as a template for this code.
// Code was altered on 7/9/2017 by Byron Eng <byron.eng@gmail.com> to
// measure pressure and temperature using the Adafruit MPL3115A2.

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_MPL3115A2.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_FeatherOLED.h>

Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();
Adafruit_FeatherOLED oled = Adafruit_FeatherOLED();

#if defined(ESP8266)
  #define BUTTON_A 0
  #define BUTTON_B 16
  #define BUTTON_C 2
  #define LED      0
#elif defined(ESP32)
  #define BUTTON_A 15
  #define BUTTON_B 32
  #define BUTTON_C 14
  #define LED      13
#elif defined(ARDUINO_STM32F2_FEATHER)
  #define BUTTON_A PA15
  #define BUTTON_B PC7
  #define BUTTON_C PC5
  #define LED PB5
#elif defined(TEENSYDUINO)
  #define BUTTON_A 4
  #define BUTTON_B 3
  #define BUTTON_C 8
  #define LED 13
#elif defined(ARDUINO_FEATHER52)
  #define BUTTON_A 31
  #define BUTTON_B 30
  #define BUTTON_C 27
  #define LED 17
#else // 32u4, M0, and 328p
  #define BUTTON_A 9
  #define BUTTON_B 6
//  #define BUTTON_C 5
  #define LED      13
#endif

const int numReadings = 10;
float pascals[numReadings];
float total = 0;
int readIndex = 0;
int units = 1;
float pressureDisplay;
float tempDisplay;

void setup()
{
  Serial.begin(115200);

  oled.init();
  oled.setBatteryVisible(true);
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    oled.setCursor(0,0);
    oled.println("Initializing...");
    oled.setCursor(0,25);
    oled.print(thisReading+1);
    oled.print(" of ");
    oled.print(numReadings);
    pascals[thisReading] = baro.getPressure();
    oled.setCursor(60,25);
    oled.print(pascals[thisReading]);oled.println(" Pa");
    total += pascals[thisReading];
    oled.display();
    delay(1000);
    oled.clearDisplay();
  }

  pinMode(BUTTON_A, INPUT);
  pinMode(BUTTON_B, INPUT);
//  pinMode(BUTTON_C, INPUT_PULLUP);
}

void loop()
{
  if (! digitalRead(BUTTON_A)) {
    units += 1; //units = 1 to see pressure in Pa and temperature in C 
    if(units > 3) units = 1;
  }
  if (!digitalRead(BUTTON_B)) {
    units -= 1; //units = 2 to see pressure in mmHg and temperature in F
    if(units < 1) units = 3;
  }
//  if (digitalRead(BUTTON_C)) units = 3; //units = 3 to see pressure in inHg and temperature in K
      // Sample code: displays "A" if button A is being pressed
      //   if (! digitalRead(BUTTON_A)) display.print("A");
  // clear the current count
  oled.clearDisplay();

  if (readIndex >= numReadings){
    readIndex = 0;
  }

  if (! baro.begin()) {
    oled.clearDisplay();
    oled.println("Couldnt find sensor");
    oled.display();
    return;
  }

  // get the current voltage of the battery and pressure from sensor
  float battery = getBatteryVoltage();
  float tempC = baro.getTemperature();

  total -= pascals[readIndex]; //Out with the old..
  pascals[readIndex] = baro.getPressure();
  total += pascals[readIndex]; //In with the new!

  float average = total/numReadings;

  if (units == 1){
     pressureDisplay = average;
     tempDisplay = (tempC * 1.8) + 32;
  }
  if (units == 2){
     pressureDisplay = 0.00750061561303 * average;
     tempDisplay = tempC;

  }
  if (units == 3){
     pressureDisplay = 0.000295299830714 * average;
     tempDisplay = (tempC + 273.15);
  }

  

  // update the battery icon
  oled.setBattery(battery);
  oled.renderBattery();

  // print the values to the OLED
      // Try oled.setCursor(0,0) before each print to set location of text
  oled.print(pressureDisplay);
  if (units == 1) oled.println(" Pa");
  if (units == 2) oled.println(" mmHg");
  if (units == 3) oled.println(" inHg");
  oled.println();
  oled.print(tempDisplay);
  if (units == 1) oled.println(" F");
  if (units == 2) oled.println(" C");
  if (units == 3) oled.println(" K");

  // update the display with the new count
  oled.display();

  readIndex++;

  // delay 1 second (1 second == 1000 milliseconds)
  delay(1000);

}

  #define VBATPIN A9

  float getBatteryVoltage() {

    float measuredvbat = analogRead(VBATPIN);

    measuredvbat *= 2;    // we divided by 2, so multiply back
    measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
    measuredvbat /= 1024; // convert to voltage

    return measuredvbat;

  }
