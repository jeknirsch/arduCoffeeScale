#include "CoffeeScale.h"

//###################### CoffeeScale class ######################
void CoffeeScale::begin(int relaisPin, int scaleSCK, int scaleDT) {
  _scale.begin(scaleDT, scaleSCK);
}
void CoffeeScale::grind(bool ON) {}

float CoffeeScale::readUnit() {
  float value = _scale.get_units();  //temporary
  return value;
}
void CoffeeScale::tareZero() {
  _scale.tare();
  //temp
  //_scale.set_scale(15000.0f);
}
void CoffeeScale::setUnitScale(float calibrationWeight) {
  _scale.calibrate_scale(calibrationWeight);
}
void CoffeeScale::debug(bool ON) {}

float CoffeeScale::meanValue(int timeMS, float maxError) {
  return 0;
}

//###################### Ringbuffer class ######################
void Ringbuffer::add(float value) {
  _ringBuffer[_ringIndex] = value;
  //index handling
  _ringIndex++;
  if (_ringIndex >= _ringSize) _ringIndex = 0;
}

float Ringbuffer::get(int index = -1) {
  int absolutIndex = index;
  //handle relative ring index
  if (index < 0 || index > _ringSize - 1) {
    absolutIndex = _ringIndex - 1; // if index out of bounds return last written Element
  } else {
    //convert relative ring index to absolut arraya index
    absolutIndex = (_ringIndex + index) % (_ringSize -1);
  }

  //DEPRECATED
  //check if index is out of bounds, if so return last written index
  // if (index < 0 || index >= _ringSize){
  //   if (_ringIndex == 0) index = _ringSize-1;
  //   else index = _ringIndex - 1; //_ringIndex is pointing to next free index -> return index before next data point
  // }
  return _ringBuffer[absolutIndex];
}


float Ringbuffer::mean(int range) {
  return 0;
}

int Ringbuffer::getSize() {
  return _ringSize;
}

//###################### EEPROM ######################