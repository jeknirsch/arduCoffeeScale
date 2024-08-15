#define VERSION 0.0

//pin defines
#define RELAISPIN 9  //D1
#define SCALE_SCK 7  //D7
#define SCALE_DT 8   //D8

// #define I2C_SCL 12  //D6
// #define I2C_SDA 14  //D5

#define BUTTON4 2  //D2
#define BUTTON3 3  //D3
#define BUTTON2 4  //D4
#define BUTTON1 5  //D5

//display specs
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

#include <HX711.h>
#include "CoffeeScale.h"
#include "UX.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//object defines
Ringbuffer data;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
CoffeeScale grinder;
UX mainUX;
ButtonManager buttons(BUTTON1, BUTTON2, BUTTON3, BUTTON4);


void setup() {
  Serial.begin(9600);
  Serial.println("Start");
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

  delay(200);
  display.clearDisplay();
  display.display();

  initUxStates();
}


void loop() {
  mainUX.uxLoop();
  buttons.buttonLoop();
  // displayLoopTime();


  RingbufferData lol = grinder.getData();
  data.add(lol);

  //display refresh
  static unsigned long timestamp = millis();
  if (millis() - timestamp > 10) {
    display.display();
    display.clearDisplay();
    timestamp = millis();
  }
}



void displayLoopTime() {
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.fillRect(0, 0, 32, 8, 0x00);
  display.print(uint8_t(data.get(0).timeMS - data.get(1).timeMS), DEC);
}