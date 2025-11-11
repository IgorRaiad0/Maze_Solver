// === Definições de pinos ===
const int lTrig = A0, lEcho = A1; // Sensor esquerdo
const int fTrig = A2, fEcho = A3; // Sensor frontal
const int rTrig = A4, rEcho = A5; // Sensor direito

const int lFowd = 6, lBack = 4; // Motor esquerdo (PWM no 6)
const int rFowd = 5, rBack = 7; // Motor direito (PWM no 5)

// === Parâmetros de controle ===
const int minSide = 8;     // cm - distância mínima de segurança lateral
const int idealDist = 12;  // cm - distância ideal do centro do corredor
const int tol = 2;         // cm - tolerância lateral
const int frontLimit = 5;  // cm - distância mínima frontal

//===================configurações de velocidade smars 01=========================

const int speed = 150;     
const int speed2 = 190;

//====================configurações de velocidade smars 02========================

/*const int speed = 150;     
const int speed2 = 170;*/


// === Parâmetros da rotina de decisão ===
const unsigned long turnTime90 = 250;  // tempo para 90°
const unsigned long delayMeasure = 1000; // 1s para estabilizar antes das medições

// === Função de leitura do sonar ===
long readUltrasonic(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long duration = pulseIn(echo, HIGH, 20000);
  long distance = duration * 0.034 / 2;
  if (distance == 0 || distance > 200) distance = 200;
  return distance;
}

// === Funções de movimento ===
void moveForward(int speed) {
  analogWrite(lFowd, speed);
  digitalWrite(lBack, LOW);
  analogWrite(rFowd, speed2);
  digitalWrite(rBack, LOW);
}

void moveBackward(int speed) {
  digitalWrite(lFowd, LOW);
  analogWrite(lBack, speed);
  digitalWrite(rFowd, LOW);
  analogWrite(rBack, speed2);
}

void turnLeft(int speed) {
  digitalWrite(lFowd, LOW);
  analogWrite(lBack, speed);
  analogWrite(rFowd, speed2);
  digitalWrite(rBack, LOW);
}

void turnRight(int speed) {
  analogWrite(lFowd, speed);
  digitalWrite(lBack, LOW);
  digitalWrite(rFowd, LOW);
  analogWrite(rBack, speed2);
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

  // === NOVA LÓGICA: Decisão de curva 90° + comparação ===
  // === Lógica de curva 90° com decisão de lado ===
  if (frontDist < 6) {
    Serial.println("🚧 Obstáculo detectado → rotina de decisão iniciada");
    stopRobot();
    delay(300);

    // 1️⃣ Gira 90° à esquerda e mede o lado esquerdo
    turnLeft(speed);
    delay(turnTime90);
    stopRobot();
    delay(delayMeasure);
    long leftCheck = readUltrasonic(fTrig, fEcho); // frente agora aponta para a esquerda

    // 2️⃣ Gira 180° à direita e mede o lado direito
    turnRight(speed);
    delay(turnTime90 * 2);
    stopRobot();
    delay(delayMeasure);
    long rightCheck = readUltrasonic(fTrig, fEcho); // frente agora aponta para a direita

    // 3️⃣ Retorna à posição neutra (90° à esquerda)
    turnLeft(speed);
    delay(turnTime90);
    stopRobot();

    Serial.print("🔄 Comparando -> E:");
    Serial.print(leftCheck);
    Serial.print(" D:");
    Serial.println(rightCheck);

    // 4️⃣ Decide o lado mais livre
    if (rightCheck > leftCheck + tol) {
      Serial.println("➡️ Direita está mais livre → virar 90° à DIREITA e seguir");
      turnRight(speed);
      delay(turnTime90);
      stopRobot();
      moveForward(speed);
    } 
    else if (leftCheck > rightCheck + tol) {
      Serial.println("⬅️ Esquerda está mais livre → virar 90° à ESQUERDA e seguir");
      turnLeft(speed);
      delay(turnTime90);
      stopRobot();
      moveForward(speed);
    } 

    else if (frontDist < frontLimit && leftDist < minSide && rightDist < minSide) {
    Serial.println("🚫 Beco sem saída detectado!");
    moveBackward(speed);
    delay(400); // recuar
    stopRobot();
    turnRight(speed); // virar 180°
    delay(turnTime90 * 2);
    stopRobot();
    moveForward(speed);
    delay(300);
}

    else {
      Serial.println("⬛ Distâncias semelhantes → seguir em frente");
      moveForward(speed);
    }

    delay(500);
    return; 
  }
  

  // === Prioridade 1: Obstáculo frontal ===
  if (frontDist < frontLimit) {
    Serial.println("Obstáculo à frente → ré");
    moveBackward(speed);
    delay(300);
    turnLeft(speed);
    delay(300);
    stopRobot();
    return;
  }

  // === Prioridade 2: Paredes muito próximas ===
  if (leftDist < minSide) {
    Serial.println("⬅️ Muito perto da esquerda → virar à direita");
    turnRight(speed);
    delay(200);
    stopRobot();
    return;
  }

  if (rightDist < minSide) {
    Serial.println("➡️ Muito perto da direita → virar à esquerda");
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
    Serial.println("↩️ Um pouco perto da direita → curva leve à esquerda");
    turnLeft(speed - 30);
  }
  else {
    Serial.println(" Centralizado → seguir em frente");
    moveForward(speed);
  }

  delay(100);
}
