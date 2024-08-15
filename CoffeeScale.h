#ifndef COFFEESCALE_H
#define COFFEESCALE_H

#include <HX711.h>
#include <EEPROM.h>

#define RINGBUFFERSIZE 10

struct RingbufferData {
  float sensorVal;
  unsigned long timeMS;
};

//class Prototypes
class CoffeeScale;
class Ringbuffer;
class EEPROMHandler;

//###################### CoffeScale main Class ######################
class CoffeeScale {
public:
  void begin(int relaisPin, int scaleSCK, int scaleDT);
  void grind(bool ON);
  bool isGrinding();
  void readScale();
  RingbufferData getData();
  void tareZero();
  void calibrate(float calibrationWeight = 50.0);
  String getParams();

private:
  bool setCalibration();
  bool readCalibration();
  EEPROMHandler *_eepromHandler;
  HX711 _loadcell;
  unsigned long _readTime;
  float _currVal;
  int _relaisPin;
  bool _isGrinding = false;
  float _offset;
  float _scale;

  const String _offsetAddr = "offset";
  const String _scaleAddr = "scale";
};


//###################### Ringbuffer Class ######################
class Ringbuffer {
public:
  void add(RingbufferData data);
  RingbufferData get(int index = 0);
  float mean(int range);
  int getSize();
  float getMax();
private:
  RingbufferData _ringBuffer[RINGBUFFERSIZE];  //allocate space for array
  const int _ringSize = RINGBUFFERSIZE;        //save ringSize as readonly
  int _ringIndex = -1;    
  bool isBufferFull = false;                     //inicating the next spot to write new data

  int convertIndex(unsigned int relIndex);
};


//###################### EEPROM Class ######################


class EEPROMHandler {
  //TOFU
public:
  void addFloat(String varName, float value);
  float readFloat(String varName);
  int calculateHashAdress(String varName);
private:
};

#endif