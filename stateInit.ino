UxFunction state_main;
UxFunction state_tare;

void stateMain() { 
  float unitsVal = grinder.readUnit();
  Serial.println(unitsVal);
  // display.clearDisplay();
  // display.setTextSize(3);
  // display.setCursor(20, 5);
  // String msg = String(unitsVal, 3);
  // msg += "g";
  // display.println(msg);
  // display.display();

  String buttonStateMsg = "";
  for (int i = 0; i < 4; i++){
    if (buttons.getState(i)) buttonStateMsg += "X ";
    else buttonStateMsg += "0 ";
  }

  if (buttons.getState(2)){
    Serial.println("set 50");
    grinder.setUnitScale(50);
  }

  // if (buttons.getState(3)){
  //   Serial.println("tare 0");
  //   grinder.tareZero();
  // }

  display.clearDisplay();
  display.setTextSize(3);
  display.setCursor(0, 2);
  display.println(buttonStateMsg);
  display.display();

}

void stateTare() {

  
}


void initUxStates() {
  //init start state
  mainUX.init(&state_main);

  state_main.stateFunction = &stateMain;
  state_main.nextFunction = &state_main;
  Serial.println("State functions assigned");
}
