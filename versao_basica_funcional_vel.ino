/* ==================== SENSORES DE LINHA HW871 ========= */
#define HW871_ESQ    A10
#define HW871_DIR    A13

/* ==================== SENSORES ULTRASSÔNICOS HC-SR04 ============
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

/* ==================== DRIVERS BTS7960 ======== */
#define D1_RPWM      2
#define D1_LPWM      3
#define D1_RENABLE   52
#define D1_LENABLE   51

#define D2_RPWM      6
#define D2_LPWM      5
#define D2_RENABLE   22
#define D2_LENABLE   25

/* ==================== CONSTANTES ==================== */
#define VELOCIDADE      70      // Avanço 
#define VELOCIDADE_RECUO 100    // Recuo 
#define VELOCIDADE_GIRO 150     // Gira

#define LIMIAR_BRANCO   890     // Padrão validado
#define TEMPO_RECUAR    400     // Padrão validado
#define TEMPO_GIRO      600     // Giro da borda 

/* ==================== CONSTANTES DA CAÇA (ajustadas) =========== */
#define MARGEM_EMPATE   10
#define TEMPO_GIRO_MIRA 120     // giro curto e rápido
#define TEMPO_LEITURA   20      // menos paradinha

/* ==================== ESTADOS ==================== */
#define ESTADO_AVANCAR  0
#define ESTADO_CACA     1
int estado = ESTADO_AVANCAR;

/* ==================== MEDIÇÃO DISTÂNCIA ==================== */
long medirDistancia(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);


  // Medir o eco, esperar no máximo 4oms
  long duracao = pulseIn(echoPin, HIGH, 40000);   // timeout 40ms
  if (duracao == 0) return 999; // não retornou, não tem nada na frente

  long distancia = duracao * 0.034 / 2;  // calcula a distância em cm
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

/* ============== MOVIMENTOS  ============= */
void andarFrente() {
  analogWrite(D1_LPWM, VELOCIDADE);
  analogWrite(D1_RPWM, 0);
  analogWrite(D2_RPWM, VELOCIDADE);
  analogWrite(D2_LPWM, 0);
}

void andarRe() {
  analogWrite(D1_LPWM, 0);
  analogWrite(D1_RPWM, VELOCIDADE_RECUO);
  analogWrite(D2_RPWM, 0);
  analogWrite(D2_LPWM, VELOCIDADE_RECUO);
}

void pararMotores() {
  analogWrite(D1_RPWM, 0);
  analogWrite(D1_LPWM, 0);
  analogWrite(D2_RPWM, 0);
  analogWrite(D2_LPWM, 0);
}

void girarDireita() {
  analogWrite(D1_LPWM, 0);
  analogWrite(D1_RPWM, VELOCIDADE_GIRO);
  analogWrite(D2_RPWM, VELOCIDADE_GIRO);
  analogWrite(D2_LPWM, 0);
}

void girarEsquerda() {
  analogWrite(D1_LPWM, VELOCIDADE_GIRO);
  analogWrite(D1_RPWM, 0);
  analogWrite(D2_RPWM, 0);
  analogWrite(D2_LPWM, VELOCIDADE_GIRO);
}

/* ==================== LOOP PRINCIPAL ==================== */
void loop() {
  // Leitura do sensor de cor
  int linhaEsq = analogRead(HW871_ESQ);
  int linhaDir = analogRead(HW871_DIR);

  /*
  É feita a identificação da leitura do sensor de cores, levando em conta o LIMIAR_BRANCO,
  o qual seria o valor definido para diferenciar o preto do branco  
  */
  if (linhaEsq > LIMIAR_BRANCO || linhaDir > LIMIAR_BRANCO) {
    // Se identificar branco ele chama o pararMotores que como o nome já diz para os motores
    pararMotores();
    delay(30);

    // Ele faz a rotação para trás
    andarRe();
    delay(TEMPO_RECUAR); // Está configurado atualmente com 400

    // Para novamente os motores
    pararMotores();
    delay(30);

    // Vai fazer a rotação para a direita
    girarDireita();
    delay(TEMPO_GIRO);

    // Para novamente
    pararMotores();
    delay(30);

    /* 
      Se for a primeira vez encontrando o branco,
      ele vai mudar o estado do código para o modo "Caça",
      referente a procurar o inimigo e empurrar
    */
    if (estado == ESTADO_AVANCAR) {
      Serial.println(">>> Primeira borda! Iniciando CAÇA");
      estado = ESTADO_CACA;
    }
  }

  /*
    A luta iniciou ele cai nessa condição se o sensor identificar a cor preta,
    essa condição é usada enquanto ele não faz o primeiro giro, 
    ele apenas segue em frente
  */
  else if (estado == ESTADO_AVANCAR) {
    // Só anda para frente até perceber a linha branca
    andarFrente();
    delay(50);
  }

  /* 
    O estado do código alterou, ele caiu na primeira condição encontrando a linha branca,
    encontrou a linha branca, ele recebe o estado de "Caça" então ele vai vir para essa condição 
  */
  else {
    // Traz as distâncias dos sensores ultrassônicos
    long distCentro = medirDistancia(TRIG_CENTRO, ECHO_CENTRO);
    long distEsq    = medirDistancia(TRIG_ESQ, ECHO_ESQ);
    long distDir    = medirDistancia(TRIG_DIR, ECHO_DIR);

    // Apenas para teste no Serial Monitor
    Serial.print("C:");
    Serial.print(distCentro);
    Serial.print(" E:");
    Serial.print(distEsq);
    Serial.print(" D:");
    Serial.println(distDir);

    /*
    Ele entra na condição conforme a posição/distância dos sensores
    */
    if (distEsq < distCentro - MARGEM_EMPATE && distEsq < distDir - MARGEM_EMPATE) {
      // Inimigo mais próximo à esquerda, gira esquerda 
      girarEsquerda();
      delay(TEMPO_GIRO_MIRA);
    }
    else if (distDir < distCentro - MARGEM_EMPATE && distDir < distEsq - MARGEM_EMPATE) {
      // Inimigo mais próximo à direita, gira direita
      girarDireita();
      delay(TEMPO_GIRO_MIRA);
    }
    else {
      // Inimigo à frente ou empate, avança para empurrar
      andarFrente();
      delay(100);
    }

    // Da um tempinho entre as leituras
    delay(TEMPO_LEITURA);
  }
}
