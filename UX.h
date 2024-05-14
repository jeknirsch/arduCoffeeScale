#ifndef UX_H
#define UX_H

#include "CoffeeScale.h"
#include <Adafruit_SSD1306.h>

#define BUTTON_NUMBER 4

//define function typedef for standard function pointer for easier readability
typedef void (*FunctionPointer)(void);

class ButtonManager;
//Deprecated
// struct UxFunction {
//   FunctionPointer stateFunction = nullptr;  //pointer to the loop function
//   UxFunction *nextFunction = nullptr;       //pointer to the next loop function -> change to different UxFunction struct
// };


////###################### Ux class ######################
class UX {
public:
  void init(FunctionPointer nextLoop);
  void changeLoop(FunctionPointer nextLoop);
  bool initLoop();
  void uxLoop();
private:
  //TO DO: Move buttonManger in UX class for ez use
  bool _initLoop = false;
  FunctionPointer _currentUxLoop = nullptr;
};
//###################### ButtonManager class ######################
//logic for flank detection getState returns int
// 0 button not pressed
// 1 button pressed
// 2 rising flank
// 3 falling flank
// Rising and falling flank only occur for on loop cycle indicating a change
class ButtonManager {  //button class for 4 Buttons
public:
  ButtonManager(int pin0, int pin1, int pin2, int pin3);
  void begin();
  void buttonLoop();
  unsigned int buttonPressTime(int buttonNr);
  int getState(int buttonNr);

private:
  int _pin0, _pin1, _pin2, _pin3;
  const int _debounceTimeMS = 50;                   //debounce time -> button signal refreshrate
  bool _buttonState[BUTTON_NUMBER] = { false };     //true = button pressed, false = button not pressed
  bool _lastButtonState[BUTTON_NUMBER] = { false };
  unsigned long int _buttonPressedMS[BUTTON_NUMBER] = { 0 }; //saves the timestamp a button is pressed -1 if button is released
};


//###################### UI components ######################
class UI_Element {
public:
  UI_Element(Adafruit_SSD1306 *display);
protected:
  Adafruit_SSD1306 *_display;
};

class UI_Number : public UI_Element {
public:
  UI_Number(int x, int y, int fontSize, Adafruit_SSD1306 *display);
  void drawNumber(float number, bool isFloat = true);
  void setSuffix(String suffix);
private:
  bool _float = true;  //int vs float
  int _x, _y;
  int _fontSize = 1;
  float _number;
  int _preDecimal, _postDecimal;  //handle numbers pre and post decimal poin separately to dispaly a fixed deciaml point
  String _suffix;
};

class UI_Graph : public UI_Element {
public:
  UI_Graph(int x, int y, int width, int height, Adafruit_SSD1306 *display);
  void setGraphResolution(int dataPoints);  //number of dataPoints displayed on the graph -> max datapoints = max width (px)
  void setDataBuffer(Ringbuffer *buffer);
private:
  int _x, _y, _width, _height;
  int _dataPoints = _width;  //default = width
  Ringbuffer *_dataBuffer = nullptr;
};


#endif