// ==================== DRIVER 1 — Motor ESQUERDO (Verde/Laranja) ====================
// ATENÇÃO: Motor montado ao contrário — RPWM e LPWM invertidos no código!
#define D1_RPWM      53
#define D1_LPWM      50
#define D1_RENABLE   52
#define D1_LENABLE   51

// ==================== DRIVER 2 — Motor DIREITO (Amarelo/Roxo) ====================
#define D2_RPWM      23
#define D2_LPWM      24
#define D2_RENABLE   22
#define D2_LENABLE   25

#define VELOCIDADE 150

void setup() {
  Serial.begin(9600);
  Serial.println("=== Teste com Driver 1 invertido ===");

  pinMode(D1_RPWM, OUTPUT);
  pinMode(D1_LPWM, OUTPUT);
  pinMode(D1_RENABLE, OUTPUT);
  pinMode(D1_LENABLE, OUTPUT);
  pinMode(D2_RPWM, OUTPUT);
  pinMode(D2_LPWM, OUTPUT);
  pinMode(D2_RENABLE, OUTPUT);
  pinMode(D2_LENABLE, OUTPUT);

  // Habilita ambos os drivers
  digitalWrite(D1_RENABLE, HIGH);
  digitalWrite(D1_LENABLE, HIGH);
  digitalWrite(D2_RENABLE, HIGH);
  digitalWrite(D2_LENABLE, HIGH);
}

void loop() {
  // ========== TESTE 1: AMBOS PARA FRENTE ==========
  // Driver 2: RPWM = frente (normal)
  // Driver 1: LPWM = frente (invertido, pois o motor está ao contrário)
  Serial.println("1. Ambos FRENTE");
  analogWrite(D1_LPWM, VELOCIDADE);   // ← INVERTIDO: LPWM vira "frente" pro D1
  analogWrite(D1_RPWM, 0);
  analogWrite(D2_RPWM, VELOCIDADE);   // ← Normal
  analogWrite(D2_LPWM, 0);
  delay(3000);

  pararMotores();
  delay(2000);

  // ========== TESTE 2: AMBOS PARA TRÁS ==========
  // Driver 2: LPWM = ré (normal)
  // Driver 1: RPWM = ré (invertido)
  Serial.println("2. Ambos RÉ");
  analogWrite(D1_LPWM, 0);
  analogWrite(D1_RPWM, VELOCIDADE);   // ← INVERTIDO: RPWM vira "ré" pro D1
  analogWrite(D2_RPWM, 0);
  analogWrite(D2_LPWM, VELOCIDADE);   // ← Normal
  delay(3000);

  pararMotores();
  delay(2000);

  // ========== TESTE 3: GIRO HORÁRIO ==========
  // D1 (esquerda) vai FRENTE | D2 (direita) vai RÉ
  Serial.println("3. Giro HORÁRIO");
  analogWrite(D1_LPWM, VELOCIDADE);   // D1 frente (invertido)
  analogWrite(D1_RPWM, 0);
  analogWrite(D2_RPWM, 0);
  analogWrite(D2_LPWM, VELOCIDADE);   // D2 ré
  delay(3000);

  pararMotores();
  delay(2000);

  // ========== TESTE 4: GIRO ANTI-HORÁRIO ==========
  // D1 (esquerda) vai RÉ | D2 (direita) vai FRENTE
  Serial.println("4. Giro ANTI-HORÁRIO");
  analogWrite(D1_LPWM, 0);
  analogWrite(D1_RPWM, VELOCIDADE);   // D1 ré (invertido)
  analogWrite(D2_RPWM, VELOCIDADE);   // D2 frente
  analogWrite(D2_LPWM, 0);
  delay(3000);

  pararMotores();
  delay(5000);
}

void pararMotores() {
  analogWrite(D1_RPWM, 0);
  analogWrite(D1_LPWM, 0);
  analogWrite(D2_RPWM, 0);
  analogWrite(D2_LPWM, 0);
}