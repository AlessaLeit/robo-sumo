/* ==================== DRIVERS BTS7960 ====================
 * Driver 1 — Motor ESQUERDO (Verde/Laranja) — INVERTIDO
 * Driver 2 — Motor DIREITO (Amarelo/Roxo) — NORMAL
 */
#define D1_RPWM      53
#define D1_LPWM      50
#define D1_RENABLE   52
#define D1_LENABLE   51

#define D2_RPWM      23
#define D2_LPWM      24
#define D2_RENABLE   22
#define D2_LENABLE   25

/* ==================== SENSORES DE LINHA ====================
 * Módulo na FRENTE do robô
 * S1 = exterior esquerdo | S3 = central (frente) | S5 = exterior direito
 */
#define S1  35   // Exterior esquerdo
#define S5  31   // Exterior direito

/* ==================== CONSTANTES ==================== */
#define VELOCIDADE     150
#define TEMPO_RECUAR   300    // Tempo para andar para trás (ms)
#define TEMPO_GIRO     630    // Tempo para girar (ms)

void setup() {
  Serial.begin(9600);

  // Configura motores
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

  // Configura sensores
  pinMode(S1, INPUT);
  pinMode(S5, INPUT);

  delay(500);
}

/* ==================== MOVIMENTOS ==================== */

void andarFrente() {
  analogWrite(D1_LPWM, 0);
  analogWrite(D1_RPWM, VELOCIDADE);   // D1 frente (invertido)
  analogWrite(D2_RPWM, 0);
  analogWrite(D2_LPWM, VELOCIDADE);   // D2 frente (normal)
}

void andarRe() {
  analogWrite(D1_LPWM, VELOCIDADE);   // D1 ré
  analogWrite(D1_RPWM, 0);
  analogWrite(D2_RPWM, VELOCIDADE);   // D2 ré
  analogWrite(D2_LPWM, 0);
}

void girarDireita() {  // Horário
  analogWrite(D1_LPWM, 0);
  analogWrite(D1_RPWM, VELOCIDADE);   // D1 frente
  analogWrite(D2_RPWM, VELOCIDADE);   // D2 ré
  analogWrite(D2_LPWM, 0);
}

void girarEsquerda() {  // Anti-horário
  analogWrite(D1_LPWM, VELOCIDADE);   // D1 ré
  analogWrite(D1_RPWM, 0);
  analogWrite(D2_RPWM, 0);
  analogWrite(D2_LPWM, VELOCIDADE);   // D2 frente
}

void pararMotores() {
  analogWrite(D1_RPWM, 0);
  analogWrite(D1_LPWM, 0);
  analogWrite(D2_RPWM, 0);
  analogWrite(D2_LPWM, 0);
}

/* ==================== LOOP PRINCIPAL ==================== */

void loop() {
  // Lê os sensores
  int s1 = digitalRead(S1);
  int s5 = digitalRead(S5);

  /* ============================================================
   * LÓGICA INVERTIDA (confirmada):
   * 1 = BRANCO (fora do dojo) | 0 = PRETO (dentro)
   * ============================================================ */

  // Se algum sensor detectar BRANCO (1) → recua, gira e anda
  if (s1 == 1 || s5 == 1) {
    Serial.println("Branco detectado!");

    // 1. Para um instante
    pararMotores();
    delay(200);

    // 2. Anda um pouco para trás
    Serial.println("  → Recuando");
    andarRe();
    delay(TEMPO_RECUAR);
    pararMotores();
    delay(200);

    // 3. Gira (aqui: para a direita — troque se quiser)
    Serial.println("  → Girando");
    girarDireita();
    delay(TEMPO_GIRO);
    pararMotores();
    delay(200);
  }
  else {
    // Tudo preto (0) → anda para frente
    Serial.println("Andando reto");
    andarFrente();
    delay(100);
  }
}