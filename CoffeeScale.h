#ifndef COFFEESCALE_H
#define COFFEESCALE_H

#include <HX711.h>

#define ringBufferSize 10
enum scaleState {};

class CoffeeScale {
public:
  void begin(int relaisPin, int scaleSCK, int scaleDT);
  void grind(bool ON);
  float readUnit();
  void tareZero();
  void setUnitScale(float calibrationWeight = 50.0);
  void debug(bool ON);
private:
  float meanValue(int timeMS, float maxError);  //maxError in unit

  HX711 _scale;
  bool _grind = false;
  bool _debug = false;
  float _unitScale;
  float _loadZero;
};

class Ringbuffer {
public:
  Ringbuffer();
  void add(float value);
  float get(int index);
  float mean(int range);
private:
  float *_ringBuffer;
  const int _ringSize = ringBufferSize;
  int _ringIndex = 0; //inicating the next spot to write new data
};


#endif