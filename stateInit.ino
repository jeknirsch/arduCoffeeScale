//function prototype
void stateMain();
void stateCalibrate();
void stateFinishGrind();

float targetWeight = 16.0;
float calibrateWeight = 50.0;
unsigned long int grindTimeStamp = 0;

struct UI_Layout {
  UI_Text mainNumber;
  UI_Text stateDisplay;
  UI_Text smallNumber;
  UI_Text target;
  UI_Graph graph;
  UI_Layout()
    //Initialize elements in struct constructor
    : mainNumber(RIGHT, BOTTOM, RIGHT, BOTTOM, 2, &display),
      stateDisplay(LEFT, TOP, LEFT, TOP, 1, &display),
      smallNumber(LEFT, BOTTOM, LEFT, BOTTOM, 1, &display),
      target(RIGHT, TOP, RIGHT, TOP, 1, &display),
      graph(RIGHT, BOTTOM, RIGHT, BOTTOM, 60, 20, &display, &data){};
};

UI_Layout ui;  //Create global ui handler

void stateMain() {


  //------------- init -------------
  const int mapCombos = 3;
  static ButtonMap stateMain_map[mapCombos];
  if (mainUX.initState()) {
    mainUX.setStateName("Main");

    stateMain_map[0] = { &stateManualGrind, { 0, 0, HOLD, 0 } };
    stateMain_map[1] = { &stateCalibrate, { 0, CLICK, 0, 0 } };
    stateMain_map[2] = { &stateAutoGrind, { 0, 0, CLICK, 0 } };

    ui.mainNumber.setSuffix("g");
    ui.smallNumber.setPrefix("v");
    ui.smallNumber.setSuffix("");

    ui.target.setPrefix("target: ");
    mainUX.setHomeState(&stateMain);

  } else {
    buttons.applyButtonPress(&mainUX, stateMain_map, mapCombos);
  }

  //------------- init end -------------
  // Serial.print("STATENAME: ");
  // Serial.println(mainUX.getStateName());
  // float unitsVal = grinder.readScale();

  float val = data.get().sensorVal;
  // ui.stateDisplay.setText("mainstate");
  ui.mainNumber.setText(val);
  ui.smallNumber.setText(float(VERSION));
  ui.stateDisplay.setText("Ready");
  ui.target.setText(targetWeight);
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
    stateMain_map[0] = { &stateFinishGrind, { CLICK, CLICK, CLICK, CLICK } };
    grinder.tareZero();
    grindTimeStamp = millis();
    grinder.grind(true);
    ui.stateDisplay.setText("AutoGrind");
    ui.smallNumber.setPrefix("");
    ui.smallNumber.setSuffix("s");
    ui.target.setSuffix("g");
    ui.target.setPrefix("");
  } else {
    buttons.applyButtonPress(&mainUX, stateMain_map, mapCombos);
  }
  //------------- loop init end -------------

  // float unitsVal = grinder.readScale();

  if (millis() - timeStamp >= timeout) mainUX.changeState(&stateFinishGrind);
  if (data.get().sensorVal >= targetWeight - 0.3) mainUX.changeState(&stateFinishGrind);
  ui.stateDisplay.setText("AutoGrind");
  ui.target.setText(data.get().sensorVal);
  ui.smallNumber.setText(float(millis() - timeStamp) / 1000.0);
  ui.graph.updateGraph();
}

void stateManualGrind() {
  //------------- init -------------
  const int mapCombos = 1;
  static ButtonMap stateMain_map[mapCombos];
  if (mainUX.initState()) {
    mainUX.setStateName("Grinding");
    stateMain_map[0] = { &stateFinishGrind, { 0, 0, HOLD_CLICK, 0 } };
    grindTimeStamp = millis();
    grinder.grind(true);
  } else {
    buttons.applyButtonPress(&mainUX, stateMain_map, mapCombos);
  }

  //------------- init end -------------
  ui.graph.updateGraph();
  ui.smallNumber.setText(data.get().sensorVal);
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
    ui.mainNumber.setSuffix("g");
    ui.smallNumber.setPrefix("");
    ui.smallNumber.setSuffix("s");
    grinder.grind(false);
  } else {
    buttons.applyButtonPress(&mainUX, stateMain_map, mapCombos);
  }

  //------------- loop init end -------------


  ui.stateDisplay.setText("Grind finished");
  ui.smallNumber.setText(float(grindTimeStamp / 1000.0));
  ui.mainNumber.setText(data.get().sensorVal);


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

    ui.mainNumber.setSuffix("g");
    ui.target.setPrefix("Set: ");
    ui.target.setSuffix("g");

    mainUX.setHomeState(&stateCalibrate);
    grinder.tareZero();
  } else {
    buttons.applyButtonPress(&mainUX, stateMain_map, mapCombos);
  }
  //------------- init end -------------
  ui.target.setText(calibrateWeight);
  ui.mainNumber.setText(data.get().sensorVal);
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
    //calibrate 50g
    grinder.calibrate(calibrateWeight);
  } else {
    buttons.applyButtonPress(&mainUX, stateMain_map, mapCombos);
  }
  //------------- init end -------------
  if (millis() - timeStamp >= timeout) mainUX.changeState(&stateMain);
}

void initUxStates() {
  //init start loop
  mainUX.init(&stateMain);

  // Serial.println("State functions assigned");
}