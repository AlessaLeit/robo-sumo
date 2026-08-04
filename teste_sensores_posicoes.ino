/* ==================== PINOS DOS SENSORES ==================== */

// Sensor CENTRO (frente) - sem fita
#define TRIG_CENTRO  44   
#define ECHO_CENTRO  45

// Sensor DIAGONAL ESQUERDA - fita T
#define TRIG_ESQ     37
#define ECHO_ESQ     36

// Sensor DIAGONAL DIREITA - fita R 
#define TRIG_DIR     43
#define ECHO_DIR     42

/* ==================== CONSTANTES ==================== */
#define DIST_MAX     400   // Distância máxima considerada (fora do alcance = 400)
#define INTERVALO_LEITURA 60  // Pausa entre leituras (ms) — evita interferência entre sensores

void setup() {
  Serial.begin(9600);
  Serial.println("=== Teste 3 Sensores HC-SR04 ===");
  Serial.println("Colunas: CENTRO | ESQUERDA | DIREITA (em cm)");
  Serial.println("-------------------------------------------");

  // Configura pinos do sensor CENTRO
  pinMode(TRIG_CENTRO, OUTPUT);
  pinMode(ECHO_CENTRO, INPUT);

  // Configura pinos do sensor ESQUERDA
  pinMode(TRIG_ESQ, OUTPUT);
  pinMode(ECHO_ESQ, INPUT);

  // Configura pinos do sensor DIREITA
  pinMode(TRIG_DIR, OUTPUT);
  pinMode(ECHO_DIR, INPUT);

  // Garante que todos os TRIGs começam em LOW
  digitalWrite(TRIG_CENTRO, LOW);
  digitalWrite(TRIG_ESQ, LOW);
  digitalWrite(TRIG_DIR, LOW);

  delay(500);  // Aguarda estabilizar
}

/*
 * lerSensor(trigPin, echoPin)
 * Mede a distância de um sensor HC-SR04.
 * Retorna a distância em cm (float).
 * Retorna DIST_MAX quando não há eco (nada detectado).
 */
float lerSensor(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Dispara pulso de 10 microssegundos
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Mede o tempo do eco (timeout de 30ms ≈ 5 metros)
  long duracao = pulseIn(echoPin, HIGH, 30000);

  // Sem eco → nada na frente
  if (duracao == 0) {
    return DIST_MAX;
  }

  // Converte tempo em centímetros
  float distancia = duracao * 0.034 / 2;

  // Ignora leituras inválidas (muito perto = ruído)
  if (distancia < 2) {
    return DIST_MAX;
  }
  if (distancia > DIST_MAX) {
    return DIST_MAX;
  }

  return distancia;
}

void loop() {
  // Lê o sensor do CENTRO primeiro
  float distCentro = lerSensor(TRIG_CENTRO, ECHO_CENTRO);

  // Pausa entre leituras para evitar que o eco de um sensor
  // seja captado pelo outro (interferência cruzada)
  delay(INTERVALO_LEITURA);

  // Lê o sensor da ESQUERDA
  float distEsq = lerSensor(TRIG_ESQ, ECHO_ESQ);

  delay(INTERVALO_LEITURA);

  // Lê o sensor da DIREITA
  float distDir = lerSensor(TRIG_DIR, ECHO_DIR);

  /* ============ MONTAGEM DA SAÍDA NO SERIAL ============ */

  Serial.print("CENTRO: ");
  if (distCentro < DIST_MAX) {
    Serial.print(distCentro);
    Serial.print(" cm  |  ");
  } else {
    Serial.print("  ---    |  ");
  }

  Serial.print("ESQUERDA: ");
  if (distEsq < DIST_MAX) {
    Serial.print(distEsq);
    Serial.print(" cm  |  ");
  } else {
    Serial.print("  ---    |  ");
  }

  Serial.print("DIREITA: ");
  if (distDir < DIST_MAX) {
    Serial.print(distDir);
    Serial.println(" cm");
  } else {
    Serial.println("  ---");
  }

  /* ============ INDICAÇÃO DA DIREÇÃO DETECTADA ============ */
  // Ajuda a visualizar de qual lado o objeto está
  bool temCentro = (distCentro < 40);
  bool temEsq    = (distEsq < 40);
  bool temDir    = (distDir < 40);

  if (temCentro && temEsq && temDir) {
    Serial.println(">>> OBJETO À FRENTE (todos os sensores)");
  } else if (temCentro) {
    Serial.println(">>> OBJETO NO CENTRO — atacar reto!");
  } else if (temEsq && temDir) {
    Serial.println(">>> OBJETO NOS DOIS LADOS — girar até centralizar");
  } else if (temEsq) {
    Serial.println(">>> OBJETO NA DIAGONAL ESQUERDA — girar p/ esquerda");
  } else if (temDir) {
    Serial.println(">>> OBJETO NA DIAGONAL DIREITA — girar p/ direita");
  }

  Serial.println();  // Linha em branco para facilitar leitura
  delay(200);
}