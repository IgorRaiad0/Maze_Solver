#include "Motors.h"
#include "Sensors.h"
#include "Navigation.h"

#define MOTOR_L_FOWD 6
#define MOTOR_L_BACK 4
#define MOTOR_R_FOWD 5
#define MOTOR_R_BACK 7

#define SENSOR_L_TRIG A0
#define SENSOR_L_ECHO A1
#define SENSOR_F_TRIG A2
#define SENSOR_F_ECHO A3
#define SENSOR_R_TRIG A4
#define SENSOR_R_ECHO A5

Motors motores = Motors(MOTOR_L_FOWD, MOTOR_L_BACK, MOTOR_R_FOWD, MOTOR_R_BACK);
Sensors sensores = Sensors(SENSOR_L_TRIG, SENSOR_L_ECHO, 
                           SENSOR_F_TRIG, SENSOR_F_ECHO, 
                           SENSOR_R_TRIG, SENSOR_R_ECHO);
Navigation navigation = Navigation(motores);

long distFront, distLeft, distRight;

unsigned long previousMillis = 0;
const unsigned long interval = 65; // Intervalo de 65ms para os sensores

int sensorState = 0; // 0=Lê Esquerda, 1=Lê Frente, 2=Lê Direita

void setup() {
  Serial.begin(9600);

  Serial.println("Iniciando Módulo de Motores...");
  motores.begin();

  Serial.println("Iniciando Módulo de Sensores...");
  sensores.begin();

  Serial.println("Iniciando Módulo de Navegação...");
  navigation.begin();
  
  distLeft  = 15;
  distFront = 12;
  distRight = 12;

  Serial.println("Setup completo.");
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Tempo da última execução
    switch (sensorState){
      case 0:
        distLeft  = sensores.readLeft(); // Lê o da esquerda
        Serial.print("L: "); Serial.print(distLeft);Serial.println("");
        sensorState = 1;
        break;
        
      case 1:
        distFront = sensores.readFront(); // Lê o frontal
        Serial.print("F: "); Serial.print(distFront);Serial.println("");
        sensorState = 2;
        break;
        
      case 2:
        distRight = sensores.readRight(); // Lê o da direita
        Serial.print("R: "); Serial.println(distRight);Serial.println("");
        sensorState = 0; // Volta para a esquerda
        break;
    }
    
    navigation.followRightWall(distLeft, distFront, distRight);
  }
}