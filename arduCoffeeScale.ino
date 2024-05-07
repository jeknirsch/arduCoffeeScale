#include <HX711.h>
#include "CoffeeScale.h"
#include "UX.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//pin defines
#define relaisPin 5  //D1
#define scaleSCK 15  //D8
#define scaleDT 13   //D7

#define i2cSCL 12  //D6
#define i2cSDA 14  //D5

#define button4 16  //D0
#define button3 4   //D2
#define button2 A0  //D3
#define button1 2   //D4

//display specs
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

//object defines
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
CoffeeScale grinder;
UX mainUX;
ButtonManager buttons(button1, button2, button3, button4);
//Ringbuffer buffer;


void setup() {
  Serial.begin(9600);
  Serial.println("Start");
  Wire.begin(i2cSDA, i2cSCL);  //sda, scl -> D5, D6
  grinder.begin(relaisPin, scaleSCK, scaleDT);
  pinMode(relaisPin, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true) {};
  }

  display.clearDisplay();
  display.display();

  display.setRotation(2);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 5);
  display.println("tare...");
  display.display();

  grinder.tareZero();

  delay(2000);
  display.clearDisplay();
  display.display();

  initUxStates();
  // for (int i = 0; i < 15; i++) {
  //   Serial.print("Buffer test:");
  //   buffer.add(i);
  //   Serial.print("Added ");
  //   Serial.println(i);
  //   Serial.print("Buffer ");
  //   Serial.println(buffer.get(i));
  // }

  // for (int i = 0; i < 10; i++) {
  //   Serial.println(buffer.get(i));
  // }
}


void loop() {
  mainUX.uxLoop();
  buttons.buttonLoop();
}