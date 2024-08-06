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
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
CoffeeScale grinder;
UX mainUX;
ButtonManager buttons(BUTTON1, BUTTON2, BUTTON3, BUTTON4);
Ringbuffer buffer;


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

  // int timeStamp = millis();
  // printState();
  // timeStamp = millis() - timeStamp;
  // Serial.println("Print time: " + String(timeStamp));

  delay(200);
  display.clearDisplay();
  display.display();

  initUxStates();
  Serial.println("DataBufferObjectSize: " + String(sizeof(RingbufferData)));



  // Serial.println("BufferTest");
  // RingbufferData tempData;
  // unsigned long int timeAVG = 0;
  // unsigned long int timeStamp = 0;
  // for (int i = 0; i < 150; i++) {
  //   tempData.sensorVal = (float)i;
  //   tempData.timeMS = millis();
  //   buffer.add(tempData); //takes about 8 - 12 us (empirical)
  //   //Serial.println(String(i) + " ---> t: " + String(millis()) + ", val: " + String(buffer.get(i).sensorVal) + ", raw: " + String(tempData.sensorVal));
  // }

  // Serial.println("Final Buffer");
  // for (int i = 0; i < 10; i++) {
  //   Serial.println(String(i) + " ---> t: " + String(millis()) + ", val: " + String(buffer.get(i).sensorVal));
  // }
}


void loop() {
  mainUX.uxLoop();
  buttons.buttonLoop();

  //display refresh
  static unsigned long int timestamp = millis();
  if (millis() - timestamp > 100) {
    display.display();
    timestamp = millis();
    printParams();
  }
}