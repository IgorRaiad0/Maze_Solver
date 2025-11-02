// === Definições de pinos ===
const int lTrig = A0, lEcho = A1; // Sensor esquerdo
const int fTrig = A2, fEcho = A3; // Sensor frontal
const int rTrig = A4, rEcho = A5; // Sensor direito

const int lFowd = 6, lBack = 4; // Motor esquerdo (PWM no 6)
const int rFowd = 5, rBack = 7; // Motor direito (PWM no 5)

// === Parâmetros de controle ===
const int minSide = 8;     // cm - distância mínima de segurança lateral
const int idealDist = 12;   // cm - distância ideal do centro do corredor
const int tol = 2;         // cm - tolerância lateral
const int frontLimit = 5; // cm - distância mínima frontal
const int speed = 150;     // velocidade padrão

// === Função de leitura do sonar ===
long readUltrasonic(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long duration = pulseIn(echo, HIGH, 20000); // timeout 20ms
  long distance = duration * 0.034 / 2;       // cm
  if (distance == 0 || distance > 200) distance = 200;
  return distance;
}

// === Funções de movimento ===
void moveForward(int speed) {
  analogWrite(lFowd, speed);
  digitalWrite(lBack, LOW);
  analogWrite(rFowd, speed);
  digitalWrite(rBack, LOW);
}

void moveBackward(int speed) {
  digitalWrite(lFowd, LOW);
  analogWrite(lBack, speed);
  digitalWrite(rFowd, LOW);
  analogWrite(rBack, speed);
}

void turnLeft(int speed) {
  digitalWrite(lFowd, LOW);
  analogWrite(lBack, speed);
  analogWrite(rFowd, speed);
  digitalWrite(rBack, LOW);
}

void turnRight(int speed) {
  analogWrite(lFowd, speed);
  digitalWrite(lBack, LOW);
  digitalWrite(rFowd, LOW);
  analogWrite(rBack, speed);
}

void stopRobot() {
  digitalWrite(lFowd, LOW);
  digitalWrite(lBack, LOW);
  digitalWrite(rFowd, LOW);
  digitalWrite(rBack, LOW);
}

// === Setup ===
void setup() {
  pinMode(lTrig, OUTPUT);
  pinMode(lEcho, INPUT);
  pinMode(fTrig, OUTPUT);
  pinMode(fEcho, INPUT);
  pinMode(rTrig, OUTPUT);
  pinMode(rEcho, INPUT);

  pinMode(lFowd, OUTPUT);
  pinMode(lBack, OUTPUT);
  pinMode(rFowd, OUTPUT);
  pinMode(rBack, OUTPUT);

  Serial.begin(9600);
  Serial.println("=== Seguidor de corredor com limites calibrados ===");
}

// === Variáveis de controle para curvas 90° ===
unsigned long curvaStartTime = 0;
bool inCurva = false;
int curvaStage = 0; // 0 = não iniciado, 1 = girando, 2 = avançando

// === Loop ===
void loop() {
  long leftDist = readUltrasonic(lTrig, lEcho);
  long frontDist = readUltrasonic(fTrig, fEcho);
  long rightDist = readUltrasonic(rTrig, rEcho);

  Serial.print("E:");
  Serial.print(leftDist);
  Serial.print("  F:");
  Serial.print(frontDist);
  Serial.print("  D:");
  Serial.println(rightDist);

  // === Detecta curva de 90° ===
 
  // Se estiver no processo de curva
  if (inCurva) {
    unsigned long currentMillis = millis();
    
    if (curvaStage == 1) { // fase de giro
      if (currentMillis - curvaStartTime >= 50) { // tempo de giro equivalente ao delay anterior
        moveForward(speed);
        curvaStartTime = currentMillis;
        curvaStage = 2;
      }
    } 
    else if (curvaStage == 2) { // fase de avanço
      if (currentMillis - curvaStartTime >= 400) { // tempo de avanço
        stopRobot();
        inCurva = false; // curva concluída
        curvaStage = 0;
      }
    }
    return; // evita executar o resto do loop enquanto curva
  }

  // === Detecta curva de 90° ===
  if (frontDist < frontLimit) {
    if (rightDist > (idealDist + 10)) {
      Serial.println("Curva de 90° à direita detectada");
      turnRight(speed);
      curvaStartTime = millis();
      inCurva = true;
      curvaStage = 1;
    } 
    else if (leftDist > (idealDist + 10)) {
      Serial.println("Curva de 90° à esquerda detectada");
      turnLeft(speed);
      curvaStartTime = millis();
      inCurva = true;
      curvaStage = 1;
    }
  }

  // === Prioridade 1: Obstáculo frontal ===
  if (frontDist < frontLimit) {
    Serial.println("Obstáculo à frente → ré");
    moveBackward(speed);
    delay(300);
    turnLeft(speed); // leve desvio
    delay(300);
    stopRobot();
    return;
  }

  // === Prioridade 2: Paredes muito próximas ===
  if (leftDist < minSide) {
    Serial.println(" Muito perto da esquerda → virar à direita");
    turnRight(speed);
    delay(200);
    stopRobot();
    return;
  }

  if (rightDist < minSide) {
    Serial.println(" Muito perto da direita → virar à esquerda");
    turnLeft(speed);
    delay(200);
    stopRobot();
    return;
  }

  // === Correção suave de alinhamento lateral ===
  if (leftDist < (idealDist - tol)) {
    Serial.println("↪️ Um pouco perto da esquerda → curva leve à direita");
    turnRight(speed - 30);
  }
  else if (rightDist < (idealDist - tol)) {
    Serial.println(" Um pouco perto da direita → curva leve à esquerda");
    turnLeft(speed - 30);
  }
  else {
    Serial.println(" Centralizado → seguir em frente");
    moveForward(speed);
  }

  delay(100);
}

