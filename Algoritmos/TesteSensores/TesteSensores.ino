// === Definições de pinos ===
const int lTrig = A0, lEcho = A1; // Sensor esquerdo
const int fTrig = A2, fEcho = A3; // Sensor frontal
const int rTrig = A4, rEcho = A5; // Sensor direito

// === Função de leitura do sonar ===
long readUltrasonic(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH, 20000); // tempo máximo de espera (20ms)
  long distance = duration * 0.034 / 2;       // converte para cm

  if (distance == 0 || distance > 200) distance = 200; // limite máximo
  return distance;
}

// === Setup ===
void setup() {
  pinMode(lTrig, OUTPUT);
  pinMode(lEcho, INPUT);
  pinMode(fTrig, OUTPUT);
  pinMode(fEcho, INPUT);
  pinMode(rTrig, OUTPUT);
  pinMode(rEcho, INPUT);

  Serial.begin(9600);
  Serial.println("=== Teste dos 3 sensores ultrassônicos ===");
}

// === Loop principal ===
void loop() {
  long leftDist = readUltrasonic(lTrig, lEcho);
  long frontDist = readUltrasonic(fTrig, fEcho);
  long rightDist = readUltrasonic(rTrig, rEcho);

  Serial.print("Esquerda: ");
  Serial.print(leftDist);
  Serial.print(" cm | Frente: ");
  Serial.print(frontDist);
  Serial.print(" cm | Direita: ");
  Serial.print(rightDist);
  Serial.println(" cm");

  delay(300); // pequena pausa para leitura estável
}
