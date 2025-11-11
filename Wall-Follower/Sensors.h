#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

class Sensors
{
  public:
    Sensors(byte lTrig, byte lEcho, byte fTrig, byte fEcho, byte rTrig, byte rEcho);

    void begin();
    long readLeft();
    long readFront();
    long readRight();

  private:
    byte _lTrig, _lEcho, _fTrig, _fEcho, _rTrig, _rEcho;

    long _readDistance(byte trigPin, byte echoPin);
};

#endif