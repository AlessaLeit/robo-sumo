/* ==================== SENSORES ULTRASSÔNICOS HC-SR04 ====================
 * Sensor CENTRO (frente)        - sem fita (Verde/Vermelho)
 * Sensor DIAGONAL ESQUERDA      - fita T (Branco/Amarelo)
 * Sensor DIAGONAL DIREITA       - fita R (Branco/Marrom)
 */
#define TRIG_CENTRO  44
#define ECHO_CENTRO  45

#define TRIG_ESQ     37
#define ECHO_ESQ     36

#define TRIG_DIR     43
#define ECHO_DIR     42

/* ==================== DRIVERS BTS7960 ====================
 * D1 (invertido): LPWM = frente | RPWM = ré
 * D2 (normal):    RPWM = frente | LPWM = ré
 */
#define D1_RPWM      53
#define D1_LPWM      50
#define D1_RENABLE   52
#define D1_LENABLE   51

#define D2_RPWM      23
#define D2_LPWM      24
#define D2_RENABLE   22
#define D2_LENABLE   25

/* ==================== CONSTANTES ====================
 * TEMPOS MÍNIMOS OTIMIZADOS
 */
#define VELOCIDADE      128    // Mínimo para os motores funcionarem
#define VELOCIDADE_GIRO 150    // Velocidade do giro
#define TEMPO_GIRO      150    // Tempo mínimo de giro (ms)
#define MARGEM_EMPATE   10     // Diferença (cm) p/ considerar empate
#define TEMPO_LEITURA   50     // Intervalo mínimo entre leituras (ms)

/* ==================== MEDIÇÃO ==================== */

long medirDistancia(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracao = pulseIn(echoPin, HIGH, 30000);   // timeout 30ms
  if (duracao == 0) return 999;

  long distancia = duracao * 0.034 / 2;
  return distancia;
}

void setup() {
  Serial.begin(9600);

  pinMode(TRIG_CENTRO, OUTPUT);
  pinMode(ECHO_CENTRO, INPUT);
  pinMode(TRIG_ESQ, OUTPUT);
  pinMode(ECHO_ESQ, INPUT);
  pinMode(TRIG_DIR, OUTPUT);
  pinMode(ECHO_DIR, INPUT);

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

  delay(500);
}

/* ==================== MOVIMENTOS ==================== */

void andarFrente() {
  analogWrite(D1_LPWM, VELOCIDADE);
  analogWrite(D1_RPWM, 0);
  analogWrite(D2_RPWM, VELOCIDADE);
  analogWrite(D2_LPWM, 0);
}

void pararMotores() {
  analogWrite(D1_RPWM, 0);
  analogWrite(D1_LPWM, 0);
  analogWrite(D2_RPWM, 0);
  analogWrite(D2_LPWM, 0);
}

void girarDireita() {
  analogWrite(D1_LPWM, VELOCIDADE_GIRO);
  analogWrite(D1_RPWM, 0);
  analogWrite(D2_RPWM, 0);
  analogWrite(D2_LPWM, VELOCIDADE_GIRO);
}

void girarEsquerda() {
  analogWrite(D1_LPWM, 0);
  analogWrite(D1_RPWM, VELOCIDADE_GIRO);
  analogWrite(D2_RPWM, VELOCIDADE_GIRO);
  analogWrite(D2_LPWM, 0);
}

/* ==================== LOOP PRINCIPAL ==================== */

void loop() {
  long distCentro = medirDistancia(TRIG_CENTRO, ECHO_CENTRO);
  long distEsq    = medirDistancia(TRIG_ESQ, ECHO_ESQ);
  long distDir    = medirDistancia(TRIG_DIR, ECHO_DIR);

  // 1. Objeto mais próximo à ESQUERDA → gira esquerda
  if (distEsq < distCentro - MARGEM_EMPATE && distEsq < distDir - MARGEM_EMPATE) {
    girarEsquerda();
    delay(TEMPO_GIRO);
    pararMotores();
    delay(80);
  }

  // 2. Objeto mais próximo à DIREITA → gira direita
  else if (distDir < distCentro - MARGEM_EMPATE && distDir < distEsq - MARGEM_EMPATE) {
    girarDireita();
    delay(TEMPO_GIRO);
    pararMotores();
    delay(80);
  }

  // 3. Objeto à frente (ou empate) → segue em frente
  else {
    andarFrente();
    delay(100);
  }

  delay(TEMPO_LEITURA);
}