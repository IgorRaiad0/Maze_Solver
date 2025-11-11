#include "Sensors.h"

Sensors::Sensors(byte lTrig, byte lEcho, byte fTrig, byte fEcho, byte rTrig, byte rEcho){
  _lTrig = lTrig;
  _lEcho = lEcho;
  _fTrig = fTrig;
  _fEcho = fEcho;
  _rTrig = rTrig;
  _rEcho = rEcho;
}

void Sensors::begin(){
  pinMode(_lTrig, OUTPUT);
  pinMode(_lEcho, INPUT);
  pinMode(_fTrig, OUTPUT);
  pinMode(_fEcho, INPUT);
  pinMode(_rTrig, OUTPUT);
  pinMode(_rEcho, INPUT);

  digitalWrite(_lTrig, LOW);
  digitalWrite(_fTrig, LOW);
  digitalWrite(_rTrig, LOW);
}

long Sensors::readLeft(){
  return _readDistance(_lTrig, _lEcho);
}

long Sensors::readFront(){
  return _readDistance(_fTrig, _fEcho);
}

long Sensors::readRight(){
  return _readDistance(_rTrig, _rEcho);
}

long Sensors::_readDistance(byte trigPin, byte echoPin) {
  long duration, distanceCm;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 25000);

  if (duration == 0) {
    return 1000;
  }
  
  distanceCm = duration / 58.82;
  return distanceCm;
}