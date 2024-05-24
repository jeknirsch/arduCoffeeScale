#include "CoffeeScale.h"

//###################### CoffeeScale class ######################
void CoffeeScale::begin(int relaisPin, int scaleSCK, int scaleDT) {
  _relaisPin = relaisPin;
  _scale.begin(scaleDT, scaleSCK);
  _eepromHandler = new EEPROMHandler();

  //EEPROM load offset and gain from eeprom if possible
  updateOffset_Gain();
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

float CoffeeScale::readUnit() {
  float value = _scale.get_units();  //temporary
  _readTime = millis();
  _currVal = value;
  return value;
}
void CoffeeScale::tareZero() {
  _scale.tare();
  //_scale.set_scale(15000.0f);
}
void CoffeeScale::setUnitScale(float calibrationWeight) {
  _scale.calibrate_scale(calibrationWeight);
  //update
  float offset = _scale.get_offset();
  float gain = _scale.get_scale();
  setOffset_Gain(offset, gain);
}

//reads from eeprom and updates class object
bool CoffeeScale::updateOffset_Gain() {
  //tofu: catch NaN maybe
  float scaleOffset = _eepromHandler->readFloat(_offsetAddr);
  float scaleGain = _eepromHandler->readFloat(_gainAddr);
  if (!isnan(scaleOffset) && !isnan(scaleOffset)) {
    _scaleOffset = scaleOffset;
    _scaleGain = scaleGain;
    setOffset_Gain(_scaleOffset, _scaleOffset);
    return true;
  } else {
    return false;
  }
}

//writes new offset and gain values to eeprom and updates class object
bool CoffeeScale::setOffset_Gain(float offset, float gain) {
  //write new values
  _eepromHandler->addFloat(_offsetAddr, offset);
  _eepromHandler->addFloat(_gainAddr, gain);

  //update class object
  return updateOffset_Gain();
}

String CoffeeScale::getParams() {
  float scale = _scale.get_scale();
  int offset = _scale.get_offset();
  String txt = "g: ";
  txt += String(_currVal);
  txt += ", t: ";
  txt += String(_readTime);
  txt += ", scale: ";
  txt += String(scale);
  txt += ", offset: ";
  txt += String(offset);
  return txt;
}

//###################### Ringbuffer class ######################
void Ringbuffer::add(RingbufferData value) {
  _ringBuffer[_ringIndex] = value;
  //index handling
  // Serial.println("BufferInternal ---> val: " + String(value.sensorVal) + ", buffer: " + String(_ringBuffer[_ringIndex].sensorVal));
  _ringIndex++;
  if (_ringIndex >= _ringSize) _ringIndex = 0;  //if index out of bounds wrap to 0
}

RingbufferData Ringbuffer::get(int index = -1) {
  index = convertIndex(index);
  return _ringBuffer[index];
}


float Ringbuffer::mean(int range) {
  return 0;
}

int Ringbuffer::getSize() {
  return _ringSize;
}

int Ringbuffer::convertIndex(int relIndex) {
  int absolutIndex = relIndex;
  //convert relative ring index to absolut array index
  absolutIndex = (_ringIndex + relIndex) % (_ringSize);
  // Serial.print("From relIndex " + String(relIndex) + " to (" + String(_ringIndex) + "+" + String(relIndex) + ") % (" + String(_ringSize) + ") = absIndex = " + String(absolutIndex));

  //index out of bounds catch --> return index = 0 for oob violation
  if (absolutIndex < 0 || absolutIndex >= _ringSize) absolutIndex = 0;
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
    if (varName.length() - 1 == i) key += varName[i] * 4;  //try to avoid colission by spacing
    else key += varName[i];
  }
  key = key % 1023;  //reduce to adress range

  return key;
}
