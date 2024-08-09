#ifndef UX_H
#define UX_H

#include "CoffeeScale.h"
#include <Adafruit_SSD1306.h>

#define BUTTON_NUMBER 4

//define function typedef for standard function pointer for easier readability
typedef void (*FunctionPointer)(void);

//Flank and state enum
enum ButtonStates {
  FLANK_UP = 2,
  FLANK_DOWN = 3,
  CLICK = 4,
  HOLD_CLICK = 5,
  HOLD = 6
};

enum Anchor {
  LEFT = -1,
  TOP = -1,
  CENTER = 0,
  RIGHT = 1,
  BOTTOM = 1
};

//stateStruct
struct ButtonMap {
  FunctionPointer stateFunction;
  int map[4];
};

//Class prototype
class ButtonManager;

////###################### Ux class ######################
class UX {
public:
  void init(FunctionPointer nextState);
  void changeState(FunctionPointer nextState);
  bool initState();
  void uxLoop();
  void setHomeState(FunctionPointer homeState);
  FunctionPointer getHomeState();
  void setStateName(String stateName);
  String getStateName();
private:
  //TO DO: Move buttonManger in UX class for ez use
  String _stateName;
  bool _initState = false;
  FunctionPointer _currentUxState = nullptr;
  FunctionPointer _homeState = nullptr;
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
  bool applyButtonPress(UX *buttonUX, ButtonMap *buttonMap, int length);

private:
  int _pin0, _pin1, _pin2, _pin3;
  const unsigned long int _debounceTimeMS = 50;                //debounce time -> button signal refreshrate
  int _buttonMode[BUTTON_NUMBER] = { 0 };        //
  bool _buttonState[BUTTON_NUMBER] = { false };  //true = button pressed, false = button not pressed
  bool _lastButtonState[BUTTON_NUMBER] = { false };
  unsigned long int _buttonPressedMS[BUTTON_NUMBER] = { 0 };  //saves the timestamp a button is pressed -1 if button is released
};


//###################### UI components ######################
class UI_Element {
public:
  UI_Element(Adafruit_SSD1306 *display);
protected:
  void _getDisplayAnchor();
  void _applyAnchor();
  Adafruit_SSD1306 *_display;
  int _x, _y, _w, _h;
  int _elementAnchorX, _elementAnchorY, _targetAnchorX, _targetAnchorY;
};

class UI_Text : public UI_Element {
public:
  UI_Text(int elementAnchorX, int elementAnchorY, int targetAnchorX, int targetAnchorY, int fontSize, Adafruit_SSD1306 *display);
  void setText(String text);
  void setSuffix(String suffix);
  void setPrefix(String prefix);
private:
  int _fontSize = 1;
  char *_text;
  char *_prefix;
  char *_suffix;
};

class UI_Graph : public UI_Element {
public:
  UI_Graph(int elementAnchorX, int elementAnchorY, int targetAnchorX, int width, int height, Adafruit_SSD1306 *display);
  void setGraphResolution(int dataPoints);  //number of dataPoints displayed on the graph -> max datapoints = max width (px)
  void setDataBuffer(Ringbuffer *buffer);
private:
  int _x, _y, _width, _height;
  int _dataPoints = _width;  //default = width
  Ringbuffer *_dataBuffer = nullptr;
};


#endif