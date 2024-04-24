#include "UX.h"
#include <Arduino.h>

//###################### UX Class ######################
//UX page setup
//every UX page consists of one UxFunction struct. The stateFunction acts as a loop handling every action for the corresponding UX state
//The stateFunction also dynammically changes the pointer to the next UxFunction struct according to its actions


void UX::init(UxFunction *initUxState) {
  //initialize first state
  _currentUxLoop = initUxState;  //To do use Function pointer
}


void UX::uxLoop() {

  if (_currentUxLoop != nullptr) _currentUxLoop->stateFunction();  //run selected state function
  //to do error display msg
}

//###################### ButtonManager ######################
ButtonManager::ButtonManager(int pin0, int pin1, int pin2, int pin3)
  : _pin0(pin0), _pin1(pin1), _pin2(pin2), _pin3(pin3) {}

void ButtonManager::begin(){
  pinMode(_pin0, INPUT);
  pinMode(_pin1, INPUT);
  pinMode(_pin2, INPUT);
  pinMode(_pin3, INPUT);
}

void ButtonManager::buttonLoop(){
  static int timeStamp = millis();
  if (millis() - timeStamp > _debounceTimeMS){
    //maybe cnovert to loop? pin array!
    buttonState[0] = digitalRead(_pin0);
    if (analogRead(_pin1) < 512) buttonState[1] = false; //since pin1 is connected to A0 it has to be handled separately
    else buttonState[1] = true;
    buttonState[2] = digitalRead(_pin2);
    buttonState[3] = digitalRead(_pin3);
  }
}

bool ButtonManager::getState(int button){ //returns false if index is out of bounds
  bool state = false;
  if (button >= 0 || button < ButtonNum) state = buttonState[button]; 
  return state;
}

void ButtonManager::setDebounceTime(int timeMS){
  _debounceTimeMS = timeMS;
}

