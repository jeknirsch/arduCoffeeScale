#include <stdlib.h>
#include "CoffeeScale.h"

//###################### CoffeeScale class ######################
void CoffeeScale::begin(int relaisPin, int scaleSCK, int scaleDT) {
  _relaisPin = relaisPin;
  _loadcell.begin(scaleDT, scaleSCK);
  _eepromHandler = new EEPROMHandler();

  //EEPROM load offset and gain from eeprom if possible
  readCalibration();
}
void CoffeeScale::grind(bool ON) {
  if (ON) {
    digitalWrite(_relaisPin, HIGH);
    _isGrinding = true;
    Serial.println("-------------------- GRINDING");
  } else {
    digitalWrite(_relaisPin, LOW);
  }
}

bool CoffeeScale::isGrinding() {
  return _isGrinding;
}

float CoffeeScale::readScale() {
  float value = _loadcell.get_units();
  _readTime = millis();
  _currVal = value;
  return value;
}

RingbufferData CoffeeScale::getData() {
  readScale();
  RingbufferData data;
  data.sensorVal = _currVal;
  data.timeMS = _readTime;
  return data;
}

void CoffeeScale::tareZero() {
  _loadcell.tare();
  //_loadcell.set_scale(15000.0f);
}
void CoffeeScale::calibrate(float calibrationWeight) {
  _loadcell.calibrate_scale(calibrationWeight);
  setCalibration();
}

//reads from eeprom and updates scale class object
bool CoffeeScale::readCalibration() {
  float offset = _eepromHandler->readFloat(_offsetAddr);
  float scale = _eepromHandler->readFloat(_scaleAddr);
  if (!isnan(offset) && !isnan(scale)) {
    _offset = offset;
    _scale = scale;
    _loadcell.set_offset(_offset);
    _loadcell.set_scale(_scale);
    return true;
  } else {
    return false;
  }
}

//writes new offset and scale values to eeprom and updates class object
bool CoffeeScale::setCalibration() {
  float offset = _loadcell.get_offset();
  float scale = _loadcell.get_scale();

  //write new values
  _eepromHandler->addFloat(_offsetAddr, offset);
  _eepromHandler->addFloat(_scaleAddr, scale);

  //update class object
  return readCalibration();
}

String CoffeeScale::getParams() {
  // BROKEN DONT TOUCH
  // float scale = _loadcell.get_scale();
  // int offset = _loadcell.get_offset();

  // char buffer[64];

  // Format the string into the buffer
  // sprintf(buffer, "g: %.2f, t: %lu, scale: %.2f, offset: %d", _currVal, _readTime, scale, offset);
  // String txt = String(buffer);

  // String txt = "g: ";
  // txt = txt + String(_currVal);
  // txt = txt + ", t: ";
  // txt = txt + String(_readTime);
  // txt = txt + ", scale: ";
  // txt = txt + String(scale);
  // txt = txt + ", offset: ";
  // txt = txt + String(offset);
  return "";
}

//###################### Ringbuffer class ######################
void Ringbuffer::add(RingbufferData value) {
  if (_ringIndex + 1 >= _ringSize) _ringIndex = -1;  //if index out of bounds wrap to 0
  _ringIndex++;
  _ringBuffer[_ringIndex] = value;

  // Serial.println(_ringIndex);
  // Serial.println(_ringBuffer[_ringIndex].sensorVal);
  //index handling
  // Serial.println("BufferInternal ---> val: " + String(value.sensorVal) + ", buffer: " + String(_ringBuffer[_ringIndex].sensorVal));
}

RingbufferData Ringbuffer::get(int index) {
  index = convertIndex(index);

  if (index < 0) {
    Serial.println("Adios");
    while(true){}; //BÖSE! TODO
  }
  return _ringBuffer[index];
}


float Ringbuffer::mean(int range) {
  return 0;
}

int Ringbuffer::getSize() {
  return _ringSize;
}

int Ringbuffer::convertIndex(unsigned int relIndex) {
  int absolutIndex = _ringIndex - relIndex;
  if (absolutIndex < 0) absolutIndex = _ringSize + absolutIndex;
  return absolutIndex;
}

//###################### EEPROM ######################
void EEPROMHandler::addFloat(String varName, float value) {
  int adress = calculateHashAdress(varName);
  byte data[4];
  memcpy(data, &value, sizeof(float));
  for (int i = 0; i < sizeof(float); i++) {
    EEPROM.update(adress + i, data[i]);
  }
}

float EEPROMHandler::readFloat(String varName) {
  int adress = calculateHashAdress(varName);
  float floatValue = 0;
  byte data[4];

  //read bytes from eeprom
  for (int i = 0; i < sizeof(float); i++) {
    data[i] = EEPROM.read(adress + i);
  }

  memcpy(&floatValue, data, sizeof(float));

  return floatValue;
}

int EEPROMHandler::calculateHashAdress(String varName) {
  //calculate simple hash for eeprom adress
  //this method is not very safe use with caution
  //not suited for much (and big) data
  unsigned int key = 0;
  for (int i = 0; i < varName.length(); i++) {
    if (varName.length() - 1 == i) key += varName[i] * 4;  //try to avoid collision by spacing
    else key += varName[i];
  }
  key = key % 1023;  //reduce to adress range

  return key;
}
