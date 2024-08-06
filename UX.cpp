#include "UX.h"
#include <Arduino.h>
#define HOLD_MIN 700


void UX::init(FunctionPointer initState) {
  //initialize first stat
  _currentUxState = initState;  //To do use Function pointer
  _initState = true;
}

void UX::changeState(FunctionPointer nextState) {
  if (nextState != nullptr) _currentUxState = nextState;
  _initState = true;
}

//WARNING: if not called _initState will not reset but shouldnt make any problems
bool UX::initState() {
  bool tempInit = _initState;
  _initState = false;
  return tempInit;
}

void UX::uxLoop() {

  if (_currentUxState != nullptr) _currentUxState();  //run selected state function
  //to do error display msg
}

void UX::setHomeState(FunctionPointer homeState) {
  _homeState = homeState;
}

FunctionPointer UX::getHomeState() {
  return _homeState;
}

void UX::setStateName(String stateName) {
  _stateName = stateName;
}

String UX::getStateName() {
  return _stateName;
}

//###################### ButtonManager ######################
ButtonManager::ButtonManager(int pin0, int pin1, int pin2, int pin3)
  : _pin0(pin0), _pin1(pin1), _pin2(pin2), _pin3(pin3) {}

void ButtonManager::begin() {
  //assign pinmodes for buttons
  pinMode(_pin0, INPUT);
  pinMode(_pin1, INPUT);
  pinMode(_pin2, INPUT);
  pinMode(_pin3, INPUT);
}

void ButtonManager::buttonLoop() {
  static unsigned long int timeStamp = millis();


  //read new buttonstates
  if (millis() - timeStamp > _debounceTimeMS) {
    _buttonState[0] = digitalRead(_pin0);
    _buttonState[1] = digitalRead(_pin1);
    _buttonState[2] = digitalRead(_pin2);
    _buttonState[3] = digitalRead(_pin3);
    timeStamp = millis();
  }

  for (int i = 0; i < BUTTON_NUMBER; i++) {
    //handle button pressTime
    if (getState(i) == 2) _buttonPressedMS[i] = millis();  //save current time if rising flank
    else if (getState(i) == 3) _buttonPressedMS[i] = 0;    //reset if falling

    //check for rising/falling flank
    //not pressed   --> 0
    //pressed       --> 1
    //rising flank  --> 2
    //falling flank --> 3
    if (_lastButtonState[i] != _buttonState[i]) {  //flank detected
      //rising:   last(0) - current(1) < 0 --> return 2
      //falling:  last(1) - current(0) > 0 --> return 3
      _buttonMode[i] = _lastButtonState[i] - _buttonState[i];
      if (_buttonMode[i] > 0) _buttonMode[i] = 3;
      else _buttonMode[i] = 2;
    } else {
      //no flank detected return current button state
      _buttonMode[i] = _buttonState[i];
    }

    //update _lastButtonState
    _lastButtonState[i] = _buttonState[i];
  }
}

unsigned int ButtonManager::buttonPressTime(int buttonNr) {
  //check for out of bounds return 0 if out of bounds
  unsigned long int tempTime = 0;
  if (_buttonPressedMS[buttonNr] > 0 && !(buttonNr < 0 || buttonNr >= BUTTON_NUMBER)) tempTime = millis() - _buttonPressedMS[buttonNr];  //calculate time since pressed
  return tempTime;
}

int ButtonManager::getState(int buttonNr) {  //returns -1  if index is out of bounds
  int state = -1;
  //check if index out of bounds
  if (!(buttonNr < 0 || buttonNr >= BUTTON_NUMBER)) {
    state = _buttonMode[buttonNr];
  }

  return state;
}



bool ButtonManager::applyButtonPress(UX *buttonUX, ButtonMap *buttonMap, int length) {

  bool isMatch = false;
  // bool anyMatch = false;
  int i = 0;
  //TO DO: make more efficient
  //iterate through buttonMap array and try to find matching button combo
  while ((!isMatch) && (i < length)) {

    for (int j = 0; j < 4; j++) {
      // isMatch = isMatch && ((buttonMap[i].map[j] < 0) || (buttonMap[i].map[j] == getState(j)));
      //anyMatch: matches if any -2 are pressed
      //   anyMatch = anyMatch || (buttonMap[i].map[j] == -2 && getState(j) >= 1);
      //   //isMatch: matches if pattern matches exactly ignore -1
      //   isMatch = (isMatch && ((buttonMap[i].map[j] == -1 || buttonMap[i].map[j] == getState(j))) || anyMatch);
      //   // Serial.println("ButtonMap [" + String(i) + "].map[" + String(j) + "] = " + String(buttonMap[i].map[j]) + " | getState = " + String(getState(j)));
      // }


      // Serial.println("ButtonMap [" + String(i) + "].map[" + String(j) + "] = " + String(buttonMap[i].map[j]) + " | getState = " + String(getState(j)));

      unsigned int pressTime = buttonPressTime(j);
      isMatch = isMatch || ((buttonMap[i].map[j] == CLICK && getState(j) == FLANK_DOWN && pressTime < HOLD_MIN));
      isMatch = isMatch || ((buttonMap[i].map[j] == HOLD_CLICK && getState(j) == FLANK_DOWN && pressTime >= HOLD_MIN));
      if (pressTime >= HOLD_MIN) {
        isMatch = isMatch || (buttonMap[i].map[j] == HOLD && getState(j) == 1);
      }
    }

    if (isMatch) buttonUX->changeState(buttonMap[i].stateFunction);
    i++;
  }

  return isMatch;
}


//###################### UI components ######################
UI_Element::UI_Element(Adafruit_SSD1306 *display)
  : _display(display) {}

void UI_Element::_getDisplayAnchor() {
  uint16_t cw = _display->width() / 2;
  uint16_t ch = _display->height() / 2;

  //Tofu pointer check
  _x = cw + cw * (_targetAnchorX);
  _y = ch + ch * (_targetAnchorY);
}

void UI_Element::_applyAnchor() {

  uint16_t cw = _w / 2;
  uint16_t ch = _h / 2;

  _x = _x - cw * (_elementAnchorX + 1);
  _y = _y - ch * (_elementAnchorY + 1);
}


UI_Text::UI_Text(int elementAnchorX, int elementAnchorY, int targetAnchorX, int targetAnchorY, int fontSize, Adafruit_SSD1306 *display)
  : _fontSize(fontSize), UI_Element(display) {
  _elementAnchorX = elementAnchorX;
  _elementAnchorY = elementAnchorY;
  _targetAnchorX = targetAnchorX;
  _targetAnchorY = targetAnchorY;
}

void UI_Text::setText(String text) {
  char *tmp;
  // strcpy(tmp, text.c_str());
  // strcat(tmp, _suffix);
  // _text = _prefix + text.c_str() + _suffix;
  _text = text.c_str();
  // Serial.print("Element coords x: ");
  // Serial.print(_x);
  // Serial.print(", y: ");
  // Serial.print(_y);
  // Serial.print(", text: ");
  // Serial.println(tmp);


  int16_t x1 = 0;
  int16_t y1 = 0;
  _w = 0;
  _h = 0;
  _display->setTextSize(_fontSize);
  _display->getTextBounds(text, (int16_t)&_x, (int16_t)&_y, &x1, &y1, (uint16_t *)&_w, (uint16_t *)&_h);
  _getDisplayAnchor();
  _applyAnchor();
  _display->setCursor(_x, _y);
  _display->println(_text);
}

void UI_Text::setSuffix(String suffix) {
  _suffix = suffix.c_str();
}

void UI_Text::setPrefix(String prefix) {
  _prefix = prefix.c_str();
}



UI_Graph::UI_Graph(int elementAnchorX, int elementAnchorY, int targetAnchorX, int width, int height, Adafruit_SSD1306 *display)
  : UI_Element(display) {
}

void UI_Graph::setGraphResolution(int dataPoints) {
  _dataPoints = dataPoints;
}

void UI_Graph::setDataBuffer(Ringbuffer *buffer) {
  _dataBuffer = buffer;
}
