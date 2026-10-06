/* ==================== SENSORES DE LINHA HW871 ========= */
#define HW871_ESQ    A10
#define HW871_DIR    A13

/* ==================== SENSORES ULTRASSÔNICOS HC-SR04 ============ */
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

/* ==================== CONSTANTES (padrão validado) =========== */
#define VELOCIDADE       70
#define VELOCIDADE_RECUO 100
#define VELOCIDADE_GIRO  150

#define LIMIAR_BRANCO    890
#define TEMPO_RECUAR     400
#define TEMPO_GIRO_180   660

/* ==================== NOVAS CONSTANTES ANTI-FANTASMA =========== */
#define LEITURAS_BORDA   2
#define TEMPO_TRAVA_DIR  320

/* ==================== CONSTANTES DA CAÇA (AJUSTADAS) ============ */
#define MARGEM_EMPATE    10
#define TEMPO_GIRO_MIRA  150
#define TEMPO_LEITURA    20

#define VELOCIDADE_ATAQUE 150
#define DIST_ATAQUE       15
#define DIST_PROCURAR     100
#define LIMITE_ECO        25000

/* ==================== ESTADOS ==================== */
#define ESTADO_AVANCAR  0
#define ESTADO_CACA     1
int estado = ESTADO_AVANCAR;

/* ==================== MEMÓRIA DA CAÇA ==================== */
int direcaoLembrada = 0;          // 0 = nenhuma, 1 = esquerda, 2 = direita
int sentidoVarredura = 1;         // +1 gira esq, -1 gira dir (alterna)
unsigned long ultimaTrocaVarredura = 0;

int ultimaDirecao = 0;            // 0 = nenhuma, 1 = esq, 2 = dir
unsigned long tempoUltimaDirecao = 0;

int contadorBorda = 0;

long ultimoCentro = 999, ultimoEsq = 999, ultimoDir = 999;

/* ==================== MEDIÇÃO ============ */
long medirDistancia(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracao = pulseIn(echoPin, HIGH, LIMITE_ECO);
  if (duracao == 0) return 999;

  return duracao * 0.034 / 2;
}

/* Filtro anti-ruído */
long filtrarSalto(long novo, long &anterior) {
  if (novo == 999) return novo;
  if (anterior != 999 && (novo > anterior + 100 || novo < anterior - 100)) {
    return anterior;
  }
  anterior = novo;
  return novo;
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

  Serial.println("=== DEBUG SUMÔ ATIVADO ===");
  Serial.println("Legenda: C=centro E=esq D=dir | Ação executada");
}

/* ============== MOVIMENTOS (mapeamento validado, NÃO MEXER) ============= */
void andarFrente() {
  analogWrite(D1_LPWM, VELOCIDADE);
  analogWrite(D1_RPWM, 0);
  analogWrite(D2_RPWM, VELOCIDADE);
  analogWrite(D2_LPWM, 0);
}

void andarFrenteAtaque() {
  analogWrite(D1_LPWM, VELOCIDADE_ATAQUE);
  analogWrite(D1_RPWM, 0);
  analogWrite(D2_RPWM, VELOCIDADE_ATAQUE);
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
  int linhaEsq = analogRead(HW871_ESQ);
  int linhaDir = analogRead(HW871_DIR);

  /* ============================================================
   * BORDA BRANCA — parada IMEDIATA + debounce rápido
   * ============================================================ */
  if (linhaEsq > LIMIAR_BRANCO || linhaDir > LIMIAR_BRANCO) {
    if (contadorBorda == 0) {
      pararMotores();
      Serial.print("BORDA! LE:");
      Serial.print(linhaEsq);
      Serial.print(" LD:");
      Serial.print(linhaDir);
      Serial.print(" contador:");
      Serial.println(contadorBorda);
    }
    contadorBorda++;

    if (contadorBorda >= LEITURAS_BORDA) {
      Serial.println(">> BORDA CONFIRMADA — recuando");
      delay(30);

      andarRe();
      delay(TEMPO_RECUAR);
      pararMotores();
      delay(30);

      Serial.println(">> Girando 180°");
      girarDireita();
      delay(TEMPO_GIRO_180);
      pararMotores();
      delay(30);

      contadorBorda = 0;

      if (estado == ESTADO_AVANCAR) {
        Serial.println(">>> Primeira borda! Girou 180°. Iniciando CAÇA");
        estado = ESTADO_CACA;
      }
    }
  } else {
    contadorBorda = 0;
  }

  /* ============================================================
   * FASE 1 — avança reto até a borda
   * ============================================================ */
  if (estado == ESTADO_AVANCAR && contadorBorda == 0) {
    andarFrente();
    delay(50);
  }

  /* ============================================================
   * FASE 2 — CAÇA (com DEBUG de decisão)
   * ============================================================ */
  else if (estado == ESTADO_CACA && contadorBorda == 0) {
    long distCentro = filtrarSalto(medirDistancia(TRIG_CENTRO, ECHO_CENTRO), ultimoCentro);
    long distEsq    = filtrarSalto(medirDistancia(TRIG_ESQ, ECHO_ESQ), ultimoEsq);
    long distDir    = filtrarSalto(medirDistancia(TRIG_DIR, ECHO_DIR), ultimoDir);

    /* ---------- 1) NADA DETECTADO → VARRE procurando ---------- */
    if (distCentro > DIST_PROCURAR && distEsq > DIST_PROCURAR && distDir > DIST_PROCURAR) {
      Serial.print("C:");
      Serial.print(distCentro);
      Serial.print(" E:");
      Serial.print(distEsq);
      Serial.print(" D:");
      Serial.print(distDir);
      Serial.print(" | VARRE (dirLembrada:");
      Serial.print(direcaoLembrada);
      Serial.print(" sentido:");
      Serial.println(sentidoVarredura);

      if (direcaoLembrada == 1) {
        girarEsquerda();
      }
      else if (direcaoLembrada == 2) {
        girarDireita();
      }
      else {
        if (millis() - ultimaTrocaVarredura > 1500) {
          ultimaTrocaVarredura = millis();
          sentidoVarredura = -sentidoVarredura;
        }
        if (sentidoVarredura > 0) girarEsquerda();
        else girarDireita();
      }
    }

    /* ---------- 2) OBJETO MAIS PRÓXIMO À ESQUERDA → gira esquerda ---------- */
    else if (distEsq < distCentro - MARGEM_EMPATE && distEsq < distDir - MARGEM_EMPATE) {
      Serial.print("C:");
      Serial.print(distCentro);
      Serial.print(" E:");
      Serial.print(distEsq);
      Serial.print(" D:");
      Serial.print(distDir);
      Serial.println(" | GIRA ESQUERDA");
      direcaoLembrada = 1;
      if (ultimaDirecao != 1 || millis() - tempoUltimaDirecao > TEMPO_TRAVA_DIR) {
        girarEsquerda();
        delay(TEMPO_GIRO_MIRA);
        ultimaDirecao = 1;
        tempoUltimaDirecao = millis();
      } else {
        girarEsquerda();
        delay(TEMPO_GIRO_MIRA);
      }
    }

    /* ---------- 3) OBJETO MAIS PRÓXIMO À DIREITA → gira direita ---------- */
    else if (distDir < distCentro - MARGEM_EMPATE && distDir < distEsq - MARGEM_EMPATE) {
      Serial.print("C:");
      Serial.print(distCentro);
      Serial.print(" E:");
      Serial.print(distEsq);
      Serial.print(" D:");
      Serial.print(distDir);
      Serial.println(" | GIRA DIREITA");
      direcaoLembrada = 2;
      if (ultimaDirecao != 2 || millis() - tempoUltimaDirecao > TEMPO_TRAVA_DIR) {
        girarDireita();
        delay(TEMPO_GIRO_MIRA);
        ultimaDirecao = 2;
        tempoUltimaDirecao = millis();
      } else {
        girarDireita();
        delay(TEMPO_GIRO_MIRA);
      }
    }

    /* ---------- 4) CENTRO MAIS PRÓXIMO (ou empate) → FOCA E VAI EM FRENTE ---------- */
    else {
      ultimaDirecao = 0;
      if (distCentro < DIST_ATAQUE) {
        Serial.print("C:");
        Serial.print(distCentro);
        Serial.print(" E:");
        Serial.print(distEsq);
        Serial.print(" D:");
        Serial.print(distDir);
        Serial.println(" | ATACA (boost)");
        andarFrenteAtaque();
        delay(40);
      } else {
        Serial.print("C:");
        Serial.print(distCentro);
        Serial.print(" E:");
        Serial.print(distEsq);
        Serial.print(" D:");
        Serial.print(distDir);
        Serial.println(" | AVANÇA");
        andarFrente();
        delay(60);
      }
    }

    delay(TEMPO_LEITURA);
  }
}
