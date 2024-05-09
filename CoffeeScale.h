#ifndef COFFEESCALE_H
#define COFFEESCALE_H

#include <HX711.h>

#define ringBufferSize 10

//###################### CoffeScale main Class ######################
class CoffeeScale {
public:
  void begin(int relaisPin, int scaleSCK, int scaleDT);
  void grind(bool ON);
  bool isGrinding();
  float readUnit();
  void tareZero();
  void setUnitScale(float calibrationWeight = 50.0);
  String getParams();
private:
  float meanValue(int timeMS, float maxError);  //maxError in unit

  HX711 _scale;
  int _readTime;
  float _currVal;
  int _relaisPin;
  bool _isGrinding = false;
  float _unitScale;
  float _loadZero;
};


//###################### Ringbuffer Class ######################
class Ringbuffer {
public:
  void add(float value);
  float get(int index);
  float mean(int range);
  int getSize();
private:
  float _ringBuffer[ringBufferSize];    //allocate space for array
  const int _ringSize = ringBufferSize; //save ringSize as readonly
  int _ringIndex = 0;                   //inicating the next spot to write new data
};


//###################### EEPROM Class ######################


class EEPROMHandler {
  //TO-DO
};

#endif