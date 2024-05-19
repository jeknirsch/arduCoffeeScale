//function prototype
void stateMain();
void stateCalibrate();
void stateFinishGrind();

float targetWeight = 16.0;
int grindTimeStamp = 0;

struct UI_Layout {
  UI_Number mainNumber;
  UI_Layout()
    : mainNumber(0, 0, 3, &display){};  //Initialize elements in struct constructor
};

UI_Layout ui;  //Create global ui handler

void stateMain() {
  //------------- init -------------
  const int mapCombos = 2;
  static ButtonMap stateMain_map[mapCombos];
  if (mainUX.initLoop()) {
    // do init stuff every time loop is freshly called
    stateMain_map[0] = { &stateStartGrind, { 0, 0, FLANK_UP, 0 } };
    stateMain_map[1] = { &stateCalibrate, { 0, FLANK_DOWN, 0, 0 } };

    display.clearDisplay();
  }
  buttons.applyButtonPress(&mainUX, stateMain_map, mapCombos);

  //------------- init end -------------

  float unitsVal = grinder.readUnit();
  display.clearDisplay();
  ui.mainNumber.setSuffix("g");
  ui.mainNumber.drawNumber(unitsVal);


  // for (int i = 0; i < 4; i++){
  //   Serial.println("Button " + String(i) + ": " + String(buttons.getState(i)));
  // }
  // ------------------------------------ Loop ------------------------------------

  // printParams();

  // int grindDuration = 60000;  //timeout
  // if (grinder.isGrinding() && millis() - grindTimeStamp > grindDuration) {
  //   grinder.grind(false);
  // }

  // if (grinder.isGrinding() && unitsVal >= targetWeight) {
  //   grinder.grind(false);
  // }
  // ----------------------------------- Loop End -----------------------------------

  // String buttonStateMsg = "";
  // for (int i = 0; i < 4; i++) {
  //   if (buttons.getState(i)) buttonStateMsg += "X ";
  //   else buttonStateMsg += "0 ";
  // }

  // if ((buttons.getState(0) == 1) && !grinder.isGrinding()) {
  //   grinder.tareZero();
  //   grinder.grind(true);
  //   grindTimeStamp = millis();
  // }

  // if (buttons.getState(2) == 3) {  //Change to calibrate page
  //   // Serial.println("set 50");
  //   // grinder.setUnitScale(50);
  //   Serial.println("Change to calibrate page");
  //   mainUX.changeLoop(&stateCalibrate);
  // }

  // if (buttons.getState(1) == 1) {
  //   //Serial.println("tare 0");
  //   grinder.tareZero();
  // }


  Serial.println("State: mainState");
}

void stateStartGrind() {
  //------------- init -------------
  const int mapCombos = 1;
  static ButtonMap stateMain_map[mapCombos];
  if (mainUX.initLoop()) {
    // do init stuff every time loop is freshly called
    stateMain_map[0] = { &stateFinishGrind, { -1, -1, 0, -1 } };

    grinder.grind(true);
    display.clearDisplay();
  }
  buttons.applyButtonPress(&mainUX, stateMain_map, mapCombos);

  //------------- init end -------------
  display.setCursor(30, 5);
  display.setTextSize(2);
  display.println("grinding...");
  Serial.println("State: grindingState");
}


void stateFinishGrind() {
  static int timeStamp;
  int returnToMain = 5000;
  //------------- loop init -------------
  const int mapCombos = 1;
  static ButtonMap stateMain_map[mapCombos];
  if (mainUX.initLoop()) {
    timeStamp = millis();
    stateMain_map[0] = { &stateMain, { -1, -1, -2, -2 } };

    grinder.grind(false);
    display.clearDisplay();
  }

  buttons.applyButtonPress(&mainUX, stateMain_map, mapCombos);

  //------------- loop init end -------------

  if (millis() - timeStamp >= returnToMain) mainUX.changeLoop(&stateMain);
  Serial.println("State: grindFinishState");
}

void stateCalibrate() {
  //------------- init -------------
  const int mapCombos = 1;
  static ButtonMap stateMain_map[mapCombos];
  if (mainUX.initLoop()) {
    // do init stuff every time loop is freshly called
    stateMain_map[0] = { &stateCalibrateApply, { 0, 0, FLANK_DOWN, 0 } };

    display.clearDisplay();
    grinder.tareZero();
  }
  buttons.applyButtonPress(&mainUX, stateMain_map, mapCombos);
  //------------- init end -------------

  display.setCursor(30, 5);
  display.setTextSize(2);
  display.println("Set 50g");

  Serial.println("State: calibrateState");
}

void stateCalibrateApply() {
  static int timeStamp;
  int timeout = 3000;
  //------------- init -------------
  const int mapCombos = 4;
  static ButtonMap stateMain_map[mapCombos];
  if (mainUX.initLoop()) {
    stateMain_map[0] = { &stateMain, { 1, 0, 0, 0 } };
    stateMain_map[1] = { &stateMain, { 0, 1, 0, 0 } };
    stateMain_map[2] = { &stateMain, { 0, 0, 1, 0 } };
    stateMain_map[3] = { &stateMain, { 0, 0, 0, 1 } };

    timeStamp = millis();
    display.clearDisplay();
    //calibrate 50g
    grinder.setUnitScale(50);
  }
  buttons.applyButtonPress(&mainUX, stateMain_map, mapCombos);
  //------------- init end -------------
  Serial.println("State: stateCalibrateApply");
  if (millis() - timeStamp >= timeout) mainUX.changeLoop(&stateMain);

  display.setCursor(30, 5);
  display.setTextSize(2);
  display.println("Calibrated");
}



void printParams() {
  Serial.println(grinder.getParams() + ", target: " + String(targetWeight) + ", isGrinding: " + String(grinder.isGrinding()));
}


void initUxStates() {
  //init start loop
  mainUX.init(&stateMain);

  Serial.println("State functions assigned");
}