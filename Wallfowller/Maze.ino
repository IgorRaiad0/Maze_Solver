#include <NewPing.h>
#include <PID_v1_bc.h>


// ====== CONFIGURAÇÕES ======
const int max_dist = 400;        // distância máxima de leitura (cm)
const double setpoint = 20.0;    // distância ideal da parede (cm)
const double Kp = 1.2, Ki = 0.3, Kd = 0.4;

const int baseSpeed = 150;
const int outLowLim = -60;
const int outHighLim = 60;

// ====== PINOS ======
const int trigPin = A0;
const int echoPin = A1;

const int lFowd = 6;  // Motor esquerdo frente (PWM)
const int lBack = 4;
const int rBack = 7;
const int rFowd = 5;  // Motor direito frente (PWM)

// ====== VARIÁVEIS ======
double distance;     // leitura atual do sonar
double outputPID;    // saída do PID

NewPing sonar(trigPin, echoPin, max_dist);
PID wallPID(&distance, &outputPID, &setpoint, Kp, Ki, Kd, DIRECT);

// ====== FUNÇÕES ======
void setup() {
  Serial.begin(9600);

  pinMode(rBack, OUTPUT);
  pinMode(rFowd, OUTPUT);
  pinMode(lBack, OUTPUT);
  pinMode(lFowd, OUTPUT);

  wallPID.SetMode(AUTOMATIC);
  wallPID.SetOutputLimits(outLowLim, outHighLim);
}

void loop() {
  readSensor();
  wallPID.Compute();
  followWall();
  printDebug();
}

// ---- Lê a distância média ----
void readSensor() {
  int iterations = 5;
  int duracao = sonar.ping_median(iterations);
  distance = sonar.convert_cm(duracao);
  if (distance == 0) distance = max_dist; // falha = longe demais
  delay(50);
}

// ---- Segue a parede ajustando as velocidades ----
void followWall() {
  double leftSpeed = baseSpeed + outputPID;
  double rightSpeed = baseSpeed - outputPID;

  leftSpeed = constrain(leftSpeed, 0, 255);
  rightSpeed = constrain(rightSpeed, 0, 255);

  analogWrite(lFowd, leftSpeed);
  digitalWrite(lBack, LOW);
  analogWrite(rFowd, rightSpeed);
  digitalWrite(rBack, LOW);
}

// ---- Debug via serial ----
void printDebug() {
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.print(" cm | PID: ");
  Serial.print(outputPID);
  Serial.print(" | LeftSpeed: ");
  Serial.print(baseSpeed + outputPID);
  Serial.print(" | RightSpeed: ");
  Serial.println(baseSpeed - outputPID);
  delay(200);
}
