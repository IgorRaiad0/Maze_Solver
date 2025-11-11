#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <Arduino.h>
#include "Motors.h"

class Navigation
{
  public:
    Navigation(Motors &motors);

    void begin();

    void followRightWall(long distLeft, long distFront, long distRight);

    void followLeftWall(long distLeft, long distFront, long distRight);
    // void pledgeAlgorithm(long distLeft, long distFront, long distRight);

  private:
    Motors &_motors;

    // --- Constantes de Ajuste ---
    const float IDEAL_RIGHT_DIST = 15.0;
    const float IDEAL_LEFT_DIST = 15.0;
    const float FRONT_BLOCK_DIST = 10.0;
    const float RIGHT_OPEN_DIST = 35.0;
    const float LEFT_OPEN_DIST = 35.0;
    const int TURN_SPEED = 180;
    const int BASE_SPEED = 180;
    const float KP_STEERING = 1.0;
};

#endif