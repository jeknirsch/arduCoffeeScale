UxFunction state_main;
UxFunction state_tare;
float time_ms = 0;

struct UI_Layout {
  UI_Number mainNumber;

  UI_Layout() : mainNumber(0, 0, 3) {}; //Initialize elements in struct constructor
};

UI_Layout ui; //Create global ui handler

void stateMain() {


  float unitsVal = grinder.readUnit();

  Serial.println(String(unitsVal) + ", " + String(millis() - time_ms));
  display.clearDisplay();
  display.setTextSize(3);
  display.setCursor(0, 5);
  String msg = String(unitsVal, 3);
  display.println(msg);
  display.display();

  String buttonStateMsg = "";
  for (int i = 0; i < 4; i++) {
    if (buttons.getState(i)) buttonStateMsg += "X ";
    else buttonStateMsg += "0 ";
  }

  if (buttons.getState(2)) {
    Serial.println("set 50");
    grinder.setUnitScale(50);
  }

  if (buttons.getState(1)) {
    Serial.println("tare 0");
    grinder.tareZero();
  }

  if (buttons.getState(0)) {
    Serial.println("set time 0");
    time_ms = millis();
  }

  // display.clearDisplay();
  // display.setTextSize(3);
  // display.setCursor(0, 2);
  // display.println(buttonStateMsg);
  // display.display();
}

void stateTare() {
}


void initUxStates() {
  //init struct

  //init start state
  mainUX.init(&state_main);

  state_main.stateFunction = &stateMain;
  state_main.nextFunction = &state_main;
  Serial.println("State functions assigned");
}