UxFunction state_main;
UxFunction state_tare;
float targetWeight = 16.0;
int grindTimeStamp = 0;

struct UI_Layout {
  UI_Number mainNumber;
  UI_Layout()
    : mainNumber(0, 0, 3, &display){};  //Initialize elements in struct constructor
};

UI_Layout ui;  //Create global ui handler

void stateMain() {
  //TO DO: automatic init function with statechange
  static int init = true;
  if (init) {

    init = false;
  }

  float unitsVal = grinder.readUnit();
  display.clearDisplay();
  ui.mainNumber.setSuffix("g");
  ui.mainNumber.drawNumber(unitsVal);

  static int timestamp = millis();
  if (millis() - timestamp > 100) {
    display.display();
    timestamp = millis();
  }
  // ------------------------------------ Loop ------------------------------------

  printState();

  int grindDuration = 60000;  //timeout
  if (grinder.isGrinding() && millis() - grindTimeStamp > grindDuration) {
    grinder.grind(false);
  }

  if (grinder.isGrinding() && unitsVal >= targetWeight) {
    grinder.grind(false);
  }
  // ----------------------------------- Loop End -----------------------------------

  // String buttonStateMsg = "";
  // for (int i = 0; i < 4; i++) {
  //   if (buttons.getState(i)) buttonStateMsg += "X ";
  //   else buttonStateMsg += "0 ";
  // }

  if (buttons.getState(0)) {
    grinder.tareZero();
    grinder.grind(true);
    grindTimeStamp = millis();
  }

  if (buttons.getState(2)) {
    Serial.println("set 50");
    grinder.setUnitScale(50);
  }

  if (buttons.getState(1)) {
    //Serial.println("tare 0");
    grinder.tareZero();
  }
}

void stateTare() {
}

void printState() {
  Serial.println(grinder.getParams() + ", target: " + String(targetWeight) + ", isGrinding: " + String(grinder.isGrinding()));
}


void initUxStates() {
  //init struct

  //init start state
  mainUX.init(&state_main);

  state_main.stateFunction = &stateMain;
  state_main.nextFunction = &state_main;
  Serial.println("State functions assigned");
}