#ifndef UX_H
#define UX_H

#include "CoffeeScale.h"
#include <Adafruit_SSD1306.h>

#define ButtonNum 4

//define function typedef for standard function pointer for easier readability
typedef void (*FunctionPointer)(void);


//ux struct
struct UxFunction {
  FunctionPointer stateFunction = nullptr;
  UxFunction *nextFunction = nullptr;
};


////###################### Ux class ######################
class UX {
public:
  void init(UxFunction *initUxState);
  void uxLoop();
private:
  UxFunction *_currentUxLoop = nullptr;
};
//###################### ButtonManager class ######################
//TO DO: Flank detection
class ButtonManager {  //button class for 4 Buttons
public:
  ButtonManager(int pin0, int pin1, int pin2, int pin3);
  void begin();
  void buttonLoop();
  int getLastPressed();
  bool getState(int button);
  void setDebounceTime(int timeMS);

private:
  int _pin0, _pin1, _pin2, _pin3;
  int _debounceTimeMS = 50;
  bool buttonState[4] = { false };  //true = button pressed, false = button not pressed
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

//###################### Gui Controller/Layout ######################

//This class defines the layout of the Display
// class UI_Controller {
// public:
//   UI_Controller();
// private:
//   UI_Number _mainNumber
// }


#endif