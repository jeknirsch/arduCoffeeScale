#include <HX711.h>
#include "CoffeeScale.h"
#include "UX.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//pin defines
#define RELAISPIN 5  //D1
#define SCALE_SCK 15  //D8
#define SCALE_DT 13   //D7

#define I2C_SCL 12  //D6
#define I2C_SDA 14  //D5

#define BUTTON4 16  //D0
#define BUTTON3 4   //D2
#define BUTTON2 A0  //D3
#define BUTTON1 2   //D4

//display specs
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

//object defines
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
CoffeeScale grinder;
UX mainUX;
ButtonManager buttons(BUTTON1, BUTTON2, BUTTON3, BUTTON4);
//Ringbuffer buffer;


void setup() {
  Serial.begin(9600);
  Serial.println("Start");
  Wire.begin(I2C_SDA, I2C_SCL);  //sda, scl -> D5, D6
  grinder.begin(RELAISPIN, SCALE_SCK, SCALE_DT);
  pinMode(RELAISPIN, OUTPUT);

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

  // int timeStamp = millis();
  // printState();
  // timeStamp = millis() - timeStamp;
  // Serial.println("Print time: " + String(timeStamp));

  delay(200);
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