#include "Navigation.h"

Navigation::Navigation(Motors &motors) : _motors(motors){
}

// Configuração inicial
void Navigation::begin(){
}

// O algoritmo de Seguidor de Parede Direita
void Navigation::followRightWall(long distLeft, long distFront, long distRight){
  // Abertura à direita?
  if (distRight > RIGHT_OPEN_DIST){
    Serial.println("NAV: Virando a Direita");
    
    // Anda um pouco para frente para alinhar
    _motors.ahead(BASE_SPEED);
    delay(300); 
    
    _motors.turnRight(); // Curva a 100% de velocidade
    delay(1000); // Tempo para realizar curva de 90
    
    _motors.stop();
    delay(100);
  }
  
  // Frente está bloqueada?
  else if (distFront <= FRONT_BLOCK_DIST){
    Serial.println("NAV: Frente bloqueada, virando à Esquerda");
    
    _motors.turnLeft(); // Curva a 100% de velocidade
    delay(1000); // Tempo para realizar curva de 90

    _motors.stop(); // Para para evitar leitura dupla
    delay(100);
  }
  
  // Segue o corredor.
  else{
    Serial.println("NAV: Seguindo Parede");
    
    // Lógica PID (Proporcional) para o corredor
    float error = IDEAL_RIGHT_DIST - distRight;
    float correction = error * KP_STEERING;

    int motor_speed_left = BASE_SPEED + correction;
    int motor_speed_right = BASE_SPEED - correction;

    // Limita as velocidades
    motor_speed_left  = constrain(motor_speed_left, 0, 210);
    motor_speed_right = constrain(motor_speed_right, 0, 210);
    Serial.println(motor_speed_right);
    Serial.println(motor_speed_left);

    // Manda as velocidades para a classe de motores
    _motors.setSpeeds(motor_speed_right, motor_speed_left);
  }
}

// O algoritmo de Seguidor de Parede Esquerda
void Navigation::followLeftWall(long distLeft, long distFront, long distRight){
  // Abertura à esquerda?
  if (distLeft > LEFT_OPEN_DIST){
    Serial.println("NAV: Virando a Esquerda");
    
    // Anda um pouco para frente para alinhar
    _motors.ahead(BASE_SPEED);
    delay(300); 
    
    _motors.turnLeft(); // Curva a 100% de velocidade
    delay(1000); // Tempo para realizar curva de 90
    
    _motors.stop();
    delay(100);
  }
  
  // Frente está bloqueada?
  else if (distFront <= FRONT_BLOCK_DIST){
    Serial.println("NAV: Frente bloqueada, virando à Direita");
    
    _motors.turnRight(); // Curva a 100% de velocidade
    delay(1000); // Tempo para realizar curva de 90

    _motors.stop(); // Para para evitar leitura dupla
    delay(100);
  }
  
  // Segue o corredor.
  else{
    Serial.println("NAV: Seguindo Parede");
    
    // Lógica PID (Proporcional) para o corredor
    float error = IDEAL_LEFT_DIST - dist;
    float correction = error * KP_STEERING;

    int motor_speed_left = BASE_SPEED + correction;
    int motor_speed_right = BASE_SPEED - correction;

    // Limita as velocidades
    motor_speed_left  = constrain(motor_speed_left, 0, 210);
    motor_speed_right = constrain(motor_speed_right, 0, 210);
    Serial.println(motor_speed_right);
    Serial.println(motor_speed_left);

    // Manda as velocidades para a classe de motores
    _motors.setSpeeds(motor_speed_right, motor_speed_left);
  }
}