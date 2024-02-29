#ifndef COFFEESCALE_H
#define COFFEESCALE_H

#include <HX711.h>
enum scaleState {};

class CoffeeScale {
public:
  CoffeeScale(int relaisPin, int scaleSCK, int scaleDT);
  void initScale();
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


#endif