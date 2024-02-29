#include <Adafruit_BME280.h>

#include <HX711.h>
#include "CoffeeScale.h"
#include "UX.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define relaisPin 10
#define scaleSCK 12
#define scaleDT 14

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
CoffeeScale grinder(relaisPin, scaleSCK, scaleDT);
HX711 scale;
UX mainUX;

Adafruit_BME280 bme;

void setup() {
  Serial.begin(9600);
  Serial.println("Start");
  scale.begin(scaleDT, scaleSCK);
  bme.begin(0x76);
  pinMode(relaisPin, OUTPUT);
  grinder.grind(true);
  mainUX.init();


  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 5);
  display.println("start");
  display.display();

  delay(2000);
  display.clearDisplay();
  display.display();

  //syntax bsp
  float temp = bme.readTemperature();
  bme.readPressure();
  bme.readHumidity();
}


float scaleRaw;
float limit = -260000;
int timeStamp = micros();
int timeDelta = 0;
float temp = 0;

void loop() {
  mainUX.uxLoop();

  scaleRaw = scale.read();

  // temp = bme.readTemperature();
  // if (scaleRaw > limit) digitalWrite(relaisPin, HIGH);
  // else digitalWrite(relaisPin, LOW);

  timeDelta = micros() - timeStamp;
  timeStamp = micros();
  // display.clearDisplay();
  // display.setCursor(20, 5);
  // display.println(scaleRaw);
  // display.setCursor(20, 20);
  // display.println(timeDelta);
  //display.display();
  Serial.println(timeDelta);
}




