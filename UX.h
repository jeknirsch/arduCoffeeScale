#ifndef UX_H
#define UX_H

#define ButtonNum 4

//define function typedef for standard function pointer for easier readability
typedef void (*FunctionPointer)(void);


//ux struct
struct UxFunction {
  FunctionPointer stateFunction = nullptr;
  UxFunction *nextFunction = nullptr;
};


////###################### Ux class ######################
class UX {
public:
  void init(UxFunction *initUxState);
  void uxLoop();
private:
  UxFunction *_currentUxLoop = nullptr;
};

//###################### UI classes ######################
class Graph {
public:
  void drawPoint();
};

class TextBox {
public:
  void drawText();
};

class UI {
public:
  void update();
  Graph graph;
  TextBox textMain;
};

//###################### ButtonManager class ######################
//button class for 4 Buttons
class ButtonManager {
public:
  ButtonManager(int pin0, int pin1, int pin2, int pin3);
  void begin();
  void buttonLoop();
  int getLastPressed();
  bool getState(int button);
  void setDebounceTime(int timeMS);

private:
  int _pin0, _pin1, _pin2, _pin3;
  int _debounceTimeMS = 50;
  bool buttonState[4] = { false };  //true = button pressed, false = button not pressed
};

#endif