/* ==================== DRIVERS BTS7960 ====================*/
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

/* ==================== SENSORES ULTRASSÔNICOS ==================== */
// Sensor CENTRO (frente) - sem fita (Verde/Vermelho)
#define TRIG_CENTRO  44   
#define ECHO_CENTRO  45

// Sensor DIAGONAL ESQUERDA - fita T (Branco/Amarelo)
#define TRIG_ESQ     37
#define ECHO_ESQ     36
 
// Sensor DIAGONAL DIREITA - fita R (Branco/Marrom)
#define TRIG_DIR     43
#define ECHO_DIR     42

/* ==================== CONSTANTES ==================== */
#define VELOCIDADE     150
#define TEMPO_GIRO     140    
#define DIST_MAX       400    // Fora do alcance
#define DIST_ALVO      40     // Distância para considerar "tem inimigo" (cm)
#define INTERVALO      60     // Pausa entre leituras (ms)

float distCentro = DIST_MAX;
float distEsq    = DIST_MAX;
float distDir    = DIST_MAX;

void setup() {
  Serial.begin(9600);
  Serial.println("=== TESTE: Sensores + Giro Direcional ===");

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
  pinMode(TRIG_CENTRO, OUTPUT);
  pinMode(ECHO_CENTRO, INPUT);
  pinMode(TRIG_ESQ, OUTPUT);
  pinMode(ECHO_ESQ, INPUT);
  pinMode(TRIG_DIR, OUTPUT);
  pinMode(ECHO_DIR, INPUT);

  digitalWrite(TRIG_CENTRO, LOW);
  digitalWrite(TRIG_ESQ, LOW);
  digitalWrite(TRIG_DIR, LOW);

  delay(500);
}

/* Lê a distância de um sensor HC-SR04 em cm */
float lerSensor(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duracao = pulseIn(echo, HIGH, 30000);
  if (duracao == 0) return DIST_MAX;

  float dist = duracao * 0.034 / 2;
  if (dist < 2 || dist > DIST_MAX) return DIST_MAX;
  return dist;
}

/* Atualiza as 3 distâncias globais */
void atualizarSensores() {
  distCentro = lerSensor(TRIG_CENTRO, ECHO_CENTRO);
  delay(INTERVALO);
  distEsq = lerSensor(TRIG_ESQ, ECHO_ESQ);
  delay(INTERVALO);
  distDir = lerSensor(TRIG_DIR, ECHO_DIR);
}

/* ==================== FUNÇÕES DOS MOTORES ==================== */

void andarFrente() {
  analogWrite(D1_LPWM, 0);
  analogWrite(D1_RPWM, VELOCIDADE);   // D1 frente (invertido)
  analogWrite(D2_RPWM, 0);
  analogWrite(D2_LPWM, VELOCIDADE);   // D2 frente (normal)
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
  atualizarSensores();

  // Mostra as leituras no Serial
  Serial.print("C:");
  Serial.print(distCentro);
  Serial.print("  E:");
  Serial.print(distEsq);
  Serial.print("  D:");
  Serial.println(distDir);

  // Decide a direção com base em qual sensor vê algo mais perto
  bool temCentro = (distCentro < DIST_ALVO);
  bool temEsq    = (distEsq < DIST_ALVO);
  bool temDir    = (distDir < DIST_ALVO);

  if (temCentro) {
    // Inimigo à frente — ataca reto
    Serial.println(">>> CENTRO — ATACANDO RETO");
    andarFrente();
    delay(2000);
    pararMotores();
  }
  else if (temEsq && temDir) {
    // Nos dois lados — compara qual está mais perto
    if (distEsq < distDir) {
      Serial.println(">>> ESQ mais perto — GIRANDO ESQUERDA");
      girarEsquerda();
      delay(TEMPO_GIRO);
    } else {
      Serial.println(">>> DIR mais perto — GIRANDO DIREITA");
      girarDireita();
      delay(TEMPO_GIRO);
    }
    pararMotores();
  }
  else if (temEsq) {
    Serial.println(">>> ESQUERDA — GIRANDO ESQUERDA");
    girarEsquerda();
    delay(TEMPO_GIRO);
    pararMotores();
  }
  else if (temDir) {
    Serial.println(">>> DIREITA — GIRANDO DIREITA");
    girarDireita();
    delay(TEMPO_GIRO);
    pararMotores();
  }
  else {
    // Nada detectado — fica parado esperando
    Serial.println(">>> NADA DETECTADO — parado");
    pararMotores();
  }

  delay(300);
}