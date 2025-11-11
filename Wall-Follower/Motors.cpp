#include "Motors.h"

Motors::Motors(byte lFowd, byte lBack, byte rFowd, byte rBack)
{
  _lFowd = lFowd;
  _lBack = lBack;
  _rFowd = rFowd;
  _rBack = rBack;
}

void Motors::begin(){
  pinMode(_lFowd, OUTPUT);
  pinMode(_lBack, OUTPUT);
  pinMode(_rFowd, OUTPUT);
  pinMode(_rBack, OUTPUT);
  stop();
}

void Motors::stop(){
  digitalWrite(_lFowd, LOW);
  digitalWrite(_lBack, LOW);
  digitalWrite(_rFowd, LOW);
  digitalWrite(_rBack, LOW);
}

void Motors::ahead(int speed){
  digitalWrite(_lBack, LOW);
  digitalWrite(_rBack, LOW);
  analogWrite(_lFowd, speed);
  analogWrite(_rFowd, speed);
}

void Motors::back(){
  analogWrite(_lFowd, 0);
  analogWrite(_rFowd, 0);
  digitalWrite(_lBack, HIGH);
  digitalWrite(_rBack, HIGH);
}

void Motors::turnRight(){
  digitalWrite(_lBack, LOW);
  analogWrite(_lFowd, 255); // Esquerdo para frente
  
  analogWrite(_rFowd, 0);
  digitalWrite(_rBack, HIGH); // Direito para trás
}

void Motors::turnLeft(){
  analogWrite(_lFowd, 0);
  digitalWrite(_lBack, HIGH); // Esquerdo para trás

  digitalWrite(_rBack, LOW);
  analogWrite(_rFowd, 255); // Direito para frente
}

void Motors::setSpeeds(int rightSpeed, int leftSpeed){
  digitalWrite(_lBack, LOW);
  digitalWrite(_rBack, LOW);

  analogWrite(_rFowd, rightSpeed); 
  analogWrite(_lFowd, leftSpeed);  
}