#define S1  A10
#define S5  A13

void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.println("=== TESTE DIGITAL x ANALOGICO ===");
  Serial.println("Passe o sensor sobre PRETO e depois sobre BRANCO.");
  Serial.println("Observe se o DIGITAL muda (0/1) e se o ANALOGICO varia.");
  Serial.println("----------------------------------------");
}

void loop() {
  // Leitura ANALÓGICA (0-1023)
  int a1 = analogRead(S1);
  int a5 = analogRead(S5);

  // Leitura DIGITAL do mesmo pino (0 ou 1)
  int d1 = digitalRead(S1);
  int d5 = digitalRead(S5);

  Serial.print("S1: analog=");
  Serial.print(a1);
  Serial.print(" digital=");
  Serial.print(d1);
  Serial.print("   |   S5: analog=");
  Serial.print(a5);
  Serial.print(" digital=");
  Serial.println(d5);

  delay(400);
}