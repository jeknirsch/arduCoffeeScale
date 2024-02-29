#include "UX.h"

//state initialization
//Create state structs
UxFunction state_main;

//create associated state functions
void stateMain(){ //currently only an example... make it more smart
    //do something

    //updating pointer to next function in struct
    state_main.nextFunction = &state_main;
};


void UX::init(){
  //initialize first state
  _currentUxLoop = &state_main;

  //initialize structs with according state functions
  state_main.stateFunction = &stateMain;
}


void UX::uxLoop() {

  if (_currentUxLoop != nullptr) _currentUxLoop->stateFunction(); //run selected state function
  else _currentUxLoop = &state_main; //point to emergency return struct
}