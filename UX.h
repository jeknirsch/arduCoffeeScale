#ifndef UX_H
#define UX_H

#define ListNum 5
#define ButtonNum 4

//define function typedef for standard function pointer for easier readability
typedef void (*FunctionPointer)(void);


//ux struct
struct UxFunction{
  FunctionPointer stateFunction = nullptr;
  UxFunction *nextFunction = nullptr;
};


//Ux class
class UX {
public:
  void init();
  void uxLoop();
private:
  UxFunction *_currentUxLoop = nullptr;
};

#endif