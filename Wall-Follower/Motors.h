#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>

class Motors{
  public:
    Motors(byte lFowd, byte lBack, byte rFowd, byte rBack);

    void begin();
    void stop();
    void ahead(int speed);
    void back();
    void turnLeft();
    void turnRight();
    void setSpeeds(int rightSpeed, int leftSpeed);

  private:
    byte _lFowd, _lBack, _rFowd, _rBack;
};

#endif