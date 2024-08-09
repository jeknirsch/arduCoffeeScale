//function prototype
void stateMain();
void stateCalibrate();
void stateFinishGrind();


float targetWeight = 16.0;
unsigned long int grindTimeStamp = 0;

struct UI_Layout {
  UI_Text mainNumber;
  UI_Text stateDisplay;
  // UI_Text smallNumber;
  // UI_Graph grindGraph;
  UI_Layout()
    //Initialize elements in struct constructor
    : mainNumber(RIGHT, CENTER, RIGHT, CENTER, 2, &display),
      stateDisplay(LEFT, TOP, LEFT, TOP, 1, &display){};
      // smallNumber(LEFT, BOTTOM, LEFT, BOTTOM, 1, &display){};
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
  // Serial.print("STATENAME: ");
  // Serial.println(mainUX.getStateName());
  // float unitsVal = grinder.readScale();
  display.clearDisplay();
  ui.mainNumber.setSuffix("g");

  float val = data.get().sensorVal;
  String txt = String(val);
  txt = txt + " g";
  Serial.println(String(val));
  ui.stateDisplay.setText("mainstate");
  ui.mainNumber.setText(txt);
}

void stateAutoGrind() {
  static unsigned long int timeStamp;
  unsigned long int timeout = 60000;
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

  // float unitsVal = grinder.readScale();

  if (millis() - timeStamp >= timeout) mainUX.changeState(&stateFinishGrind);
  if (data.get().sensorVal >= targetWeight) mainUX.changeState(&stateFinishGrind);
  display.clearDisplay();
  ui.stateDisplay.setText("AutoGrind");
  ui.mainNumber.setText(String(data.get().sensorVal) + "g");
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
  static unsigned long timeStamp = millis();
  unsigned long returnToMain = 5500;
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

  // float unitsVal = grinder.readScale();
  display.clearDisplay();
  ui.stateDisplay.setText(String("Grind Finish"/* + String(data.get().sensorVal)*/));

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
  unsigned long timeout = 10000;
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

void initUxStates() {
  //init start loop
  mainUX.init(&stateMain);

  // Serial.println("State functions assigned");
}