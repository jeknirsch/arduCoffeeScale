#include <cstring>
#include <iterator>
#include "UX.h"
#include <Arduino.h>


void UX::init(FunctionPointer initLoop) {
  //initialize first stat
  _currentUxLoop = initLoop;  //To do use Function pointer
  _initLoop = true;
}

void UX::changeLoop(FunctionPointer nextLoop){
  if (nextLoop != nullptr) _currentUxLoop = nextLoop;
  _initLoop = true;
}

//WARNING: if not called _initLoop will not reset but shouldnt make any problems
bool UX::initLoop(){
  bool tempInit = _initLoop;
  _initLoop = false;
  return tempInit;
}

void UX::uxLoop() {

  if (_currentUxLoop != nullptr) _currentUxLoop();  //run selected state function
  //to do error display msg
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
  static int timeStamp = millis();

  //copy array _buttonstate to _lastButtonState
  std::memcpy(_lastButtonState, _buttonState, BUTTON_NUMBER * sizeof(int));

  //read new buttonstates
  if (millis() - timeStamp > _debounceTimeMS) {
    _buttonState[0] = digitalRead(_pin0);
    if (analogRead(_pin1) < 512) _buttonState[1] = false;  //since pin1 is connected to A0 it has to be handled separately
    else _buttonState[1] = true;
    _buttonState[2] = digitalRead(_pin2);
    _buttonState[3] = digitalRead(_pin3);
    timeStamp = millis();
  }

  //handle button pressTime
  for (int i = 0; i < BUTTON_NUMBER; i++){
    //MAYBE: 1) a button timeout 2) reset logic save last or 0?
    if (getState(i) == 2) _buttonPressedMS[i] = millis(); //save current time if rising flank
    else if (getState(i) == 3) _buttonPressedMS[i] = 0; //reset if falling
  }
}

unsigned int ButtonManager::buttonPressTime(int buttonNr){
  //check for out of bounds return 0 if out of bounds
  unsigned long int tempTime = 0;
  if (!(buttonNr < 0 || buttonNr >= BUTTON_NUMBER)) tempTime = millis() - _buttonPressedMS[buttonNr]; //calculate time since pressed
  return tempTime;
}

int ButtonManager::getState(int buttonNr) {  //returns -1  if index is out of bounds
  int state = - 1;
  //check if index out of bounds
  if (!(buttonNr < 0 || buttonNr >= BUTTON_NUMBER)){
    //check for flank/state
    if (_lastButtonState[buttonNr] != _buttonState[buttonNr]){ //flank detected
      state = _lastButtonState[buttonNr] - _buttonState[buttonNr];
      //check for rising/falling flank
      //rising:   last(0) - current(1) < 0 --> return 2
      //falling:  last(1) - current(0) > 0 --> return 3
      if (state > 0) state = 3;
      else state = 2;
    }else{
      //no flank detected return current button state
      state = _buttonState[buttonNr];
    }
  }
  
  return state;
}

//###################### UI components ######################
UI_Element::UI_Element(Adafruit_SSD1306 *display)
  : _display(display) {}


UI_Number::UI_Number(int x, int y, int fontSize, Adafruit_SSD1306 *display)
  : _x(x), _y(y), _fontSize(fontSize), UI_Element(display) {}
void UI_Number::drawNumber(float number, bool isFloat) {
  _number = number;
  _display->setCursor(_x, _y);
  _display->setTextSize(_fontSize);
  _display->println(String(_number) + _suffix);
}
void UI_Number::setSuffix(String suffix) {
  _suffix = suffix;
}


UI_Graph::UI_Graph(int x, int y, int width, int height, Adafruit_SSD1306 *display)
  : UI_Element(display) {}
void UI_Graph::setGraphResolution(int dataPoints) {
  _dataPoints = dataPoints;
}

void UI_Graph::setDataBuffer(Ringbuffer *buffer) {
  _dataBuffer = buffer;
}

