#include "CoffeeScale.h"

//###################### CoffeeScale class ######################
void CoffeeScale::begin(int relaisPin, int scaleSCK, int scaleDT) {
    _scale.begin(scaleDT, scaleSCK);
}
void CoffeeScale::grind(bool ON) {}

float CoffeeScale::readUnit() {
  float value = _scale.get_units(); //temporary
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
Ringbuffer::Ringbuffer(){
  _ringBuffer = new float[_ringSize];
}

void Ringbuffer::add(float value){
  _ringBuffer[_ringIndex] = value;
  //index handling
  _ringIndex++;
  if (_ringIndex >= _ringSize) _ringIndex = 0;
}

float Ringbuffer::get(int index = -1){
  //check if index is out of bounds, if so return last written index
  if (index < 0 || index >= _ringSize){
    if (_ringIndex == 0) index = _ringSize-1;
    else index = _ringIndex - 1; //_ringIndex is pointing to next free index so decrease by one for last index
  }
  return _ringBuffer[index];
}


float Ringbuffer::mean(int range){
  return 0;
}