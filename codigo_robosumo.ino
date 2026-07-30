// ==================== DRIVERS BTS7960 ====================
// Driver 1 — Motor ESQUERDO (Verde/Laranja) — INVERTIDO
#define D1_RPWM      53
#define D1_LPWM      50
#define D1_RENABLE   52
#define D1_LENABLE   51

// Driver 2 — Motor DIREITO (Amarelo/Roxo) — NORMAL
#define D2_RPWM      23
#define D2_LPWM      24
#define D2_RENABLE   22
#define D2_LENABLE   25

#define VELOCIDADE     150
#define TEMPO_GIRO     630   // Ajuste para girar ~90°

void setup() {
  Serial.begin(9600);
  Serial.println("=== Teste de Alinhamento: Frente → Direita → Frente → Esquerda ===");

  pinMode(D1_RPWM, OUTPUT);
  pinMode(D1_LPWM, OUTPUT);
  pinMode(D1_RENABLE, OUTPUT);
  pinMode(D1_LENABLE, OUTPUT);
  pinMode(D2_RPWM, OUTPUT);
  pinMode(D2_LPWM, OUTPUT);
  pinMode(D2_RENABLE, OUTPUT);
  pinMode(D2_LENABLE, OUTPUT);

  digitalWrite(D1_RENABLE, HIGH);
  digitalWrite(D1_LENABLE, HIGH);
  digitalWrite(D2_RENABLE, HIGH);
  digitalWrite(D2_LENABLE, HIGH);

  delay(1000);  // 1s para preparar
}

void loop() {
  // ============ 1. FRENTE por 4s ============
  // D1 (invertido): RPWM = frente | D2 (normal): LPWM = frente
  Serial.println("1. FRENTE");
  analogWrite(D1_LPWM, 0);
  analogWrite(D1_RPWM, VELOCIDADE);
  analogWrite(D2_RPWM, 0);
  analogWrite(D2_LPWM, VELOCIDADE);
  delay(4000);

  pararMotores();
  delay(500);

  // ============ 2. GIRAR DIREITA (horário) ============
  // Lógica: esquerda (D1) frente + direita (D2) ré
  //          D1_RPWM = frente  |  D2_RPWM = ré
  Serial.println("2. GIRAR DIREITA");
  analogWrite(D1_LPWM, 0);
  analogWrite(D1_RPWM, VELOCIDADE);  // D1 frente
  analogWrite(D2_RPWM, VELOCIDADE);  // D2 ré
  analogWrite(D2_LPWM, 0);
  delay(TEMPO_GIRO);

  pararMotores();
  delay(500);

  // ============ 3. FRENTE por 4s ============
  Serial.println("3. FRENTE");
  analogWrite(D1_LPWM, 0);
  analogWrite(D1_RPWM, VELOCIDADE);
  analogWrite(D2_RPWM, 0);
  analogWrite(D2_LPWM, VELOCIDADE);
  delay(4000);

  pararMotores();
  delay(500);

  // ============ 4. GIRAR ESQUERDA (anti-horário) ============
  // Lógica: esquerda (D1) ré + direita (D2) frente
  //          D1_LPWM = ré  |  D2_LPWM = frente
  Serial.println("4. GIRAR ESQUERDA");
  analogWrite(D1_LPWM, VELOCIDADE);  // D1 ré
  analogWrite(D1_RPWM, 0);
  analogWrite(D2_RPWM, 0);
  analogWrite(D2_LPWM, VELOCIDADE);  // D2 frente
  delay(TEMPO_GIRO);

  pararMotores();
  delay(500);
}

void pararMotores() {
  analogWrite(D1_RPWM, 0);
  analogWrite(D1_LPWM, 0);
  analogWrite(D2_RPWM, 0);
  analogWrite(D2_LPWM, 0);
}