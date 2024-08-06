//function prototype
void stateMain();
void stateCalibrate();
void stateFinishGrind();


float targetWeight = 16.0;
int grindTimeStamp = 0;

struct UI_Layout {
  UI_Text mainNumber;
  UI_Text stateDisplay;
  UI_Layout()
    : mainNumber(RIGHT, CENTER, RIGHT, CENTER, 2, &display),
      stateDisplay(LEFT, TOP, LEFT, TOP, 1, &display){};  //Initialize elements in struct constructor
};

UI_Layout ui;  //Create global ui handler

void stateMain() {
  //------------- init -------------
  const int mapCombos = 3;
  static ButtonMap stateMain_map[mapCombos];
  if (mainUX.initState()) {
    mainUX.setStateName("Main");
    stateMain_map[0] = { &stateStartGrind, { 0, 0, HOLD, 0 } };
    stateMain_map[1] = { &stateCalibrate, { 0, CLICK, 0, 0 } };
    stateMain_map[2] = { &stateAutoGrind, { 0, 0, CLICK, 0 } };

    mainUX.setHomeState(&stateMain);

    display.clearDisplay();
  } else {
    buttons.applyButtonPress(&mainUX, stateMain_map, mapCombos);
  }

  //------------- init end -------------

  float unitsVal = grinder.readUnit();
  char c = 'a';
  display.clearDisplay();
  ui.mainNumber.setSuffix("g");
  ui.mainNumber.setText(String(unitsVal) + "g");
  ui.stateDisplay.setText("mainstate");
}

void stateAutoGrind() {
  static int timeStamp;
  int timeout = 60000;
  //------------- loop init -------------
  const int mapCombos = 1;
  static ButtonMap stateMain_map[mapCombos];
  if (mainUX.initState()) {
    mainUX.setStateName("Autogrind");
    timeStamp = millis();
    grindTimeStamp = millis();
    stateMain_map[0] = { &stateFinishGrind, { CLICK, CLICK, CLICK, CLICK } };
    grinder.tareZero();
    grinder.grind(true);
    display.clearDisplay();
    ui.stateDisplay.setText("AutoGrind");

  } else {
    buttons.applyButtonPress(&mainUX, stateMain_map, mapCombos);
  }
  //------------- loop init end -------------

  float unitsVal = grinder.readUnit();

  if (millis() - timeStamp >= timeout) mainUX.changeState(&stateFinishGrind);
  if (unitsVal >= targetWeight) mainUX.changeState(&stateFinishGrind);
  display.clearDisplay();
  ui.stateDisplay.setText("AutoGrind");
  ui.mainNumber.setText(String(unitsVal) + "g");
}

void stateStartGrind() {
  //------------- init -------------
  const int mapCombos = 1;
  static ButtonMap stateMain_map[mapCombos];
  if (mainUX.initState()) {
    mainUX.setStateName("Grinding");
    stateMain_map[0] = { &stateFinishGrind, { 0, 0, HOLD_CLICK, 0 } };

    grinder.grind(true);
    display.clearDisplay();
  } else {
    buttons.applyButtonPress(&mainUX, stateMain_map, mapCombos);
  }

  //------------- init end -------------
  display.setCursor(30, 5);
  display.setTextSize(2);
  display.println("grinding...");
}


void stateFinishGrind() {
  static unsigned long int timeStamp = millis();
  int returnToMain = 5500;
  //------------- loop init -------------
  const int mapCombos = 1;
  static ButtonMap stateMain_map[mapCombos];
  if (mainUX.initState()) {
    mainUX.setStateName("FinishGrind");
    timeStamp = millis();
    grindTimeStamp = millis() - grindTimeStamp;
    stateMain_map[0] = { &stateMain, { CLICK, CLICK, CLICK, 0 } };

    grinder.grind(false);
    display.clearDisplay();
  } else {
    buttons.applyButtonPress(&mainUX, stateMain_map, mapCombos);
  }

  //------------- loop init end -------------

  float unitsVal = grinder.readUnit();
  display.clearDisplay();
  ui.stateDisplay.setText(String("Grind Finish" + String(unitsVal)));

  ui.mainNumber.setText(String(grindTimeStamp / 1000.0));


  if (millis() - timeStamp >= returnToMain) {
    mainUX.changeState(&stateMain);
  }
}

void stateCalibrate() {
  //------------- init -------------
  const int mapCombos = 2;
  static ButtonMap stateMain_map[mapCombos];
  if (mainUX.initState()) {
    mainUX.setStateName("Calibrate");
    // do init stuff every time loop is freshly called
    stateMain_map[0] = { &stateCalibrateApply, { 0, 0, CLICK, 0 } };
    stateMain_map[1] = { mainUX.getHomeState(), { CLICK, CLICK, 0, CLICK } };

    mainUX.setHomeState(&stateCalibrate);

    display.clearDisplay();
    grinder.tareZero();
  } else {
    buttons.applyButtonPress(&mainUX, stateMain_map, mapCombos);
  }
  //------------- init end -------------

  display.setCursor(30, 5);
  display.setTextSize(2);
  display.println("Set 50g");
}

void stateCalibrateApply() {
  static unsigned long int timeStamp;
  int timeout = 10000;
  const int mapCombos = 1;
  static ButtonMap stateMain_map[mapCombos];

  //------------- init -------------
  if (mainUX.initState()) {
    mainUX.setStateName("CalibrateApply");
    stateMain_map[0] = { &stateMain, { CLICK, CLICK, CLICK, 0 } };

    timeStamp = millis();
    display.clearDisplay();
    //calibrate 50g
    grinder.calibrate(50);
  } else {
    buttons.applyButtonPress(&mainUX, stateMain_map, mapCombos);
  }
  //------------- init end -------------
  if (millis() - timeStamp >= timeout) mainUX.changeState(&stateMain);

  display.setCursor(30, 5);
  display.setTextSize(1);
  display.println("Calibrated");
}



void printParams() {
  Serial.print(grinder.getParams());
  Serial.print(", target: ");
  Serial.print(targetWeight);
  Serial.print(", state: ");
  Serial.println(mainUX.getStateName());
}

void initUxStates() {
  //init start loop
  mainUX.init(&stateMain);

  // Serial.println("State functions assigned");
}