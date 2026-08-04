# Robô Sumô Autônomo — Arduino

Código-fonte de um robô sumô autônomo baseado em Arduino, utilizando tração diferencial (2 rodas), sensores ultrassônicos HC-SR04 e drivers BTS7960.

##  Visão Geral
O robô executa 3 fases principais:

1. **Inicialização e busca da borda** — Avança até detectar a borda branca do dojo
2. **Manobra** — Recua e gira 180° para ficar de costas para a borda
3. **Busca e ataque** — Varre o ambiente com ultrassons e ataca o inimigo

##  Componentes

Componente - Quantidade 

- Arduino Mega 2560 - 1 
- Driver BTS7960 (Ponte H) - 2 
- Motor DC - 2 
- Sensor Ultrassônico HC-SR04 - 2 
- Sensor de Linha TCRT5000  - 1 
- Bateria externa (para motores) - 1

##  Mapeamento de Pinos

### Drivers BTS7960

#### DRIVER 1 — Motor ESQUERDO (Verde/Laranja) 

- D1_RPWM      53
- D1_LPWM      50
- D1_RENABLE   52
- D1_LENABLE   51

#### DRIVER 2 — Motor DIREITO (Amarelo/Roxo) 

- D2_RPWM      23
- D2_LPWM      24
- D2_RENABLE   22
- D2_LENABLE   25

### Sensores

#### Sensor CENTRO (frente) - sem fita
- TRIG_CENTRO = 44
- ECHO_CENTRO = 45

#### Sensor DIAGONAL ESQUERDA - fita T
- TRIG_ESQ = 37
- ECHO_ESQ = 36

#### Sensor DIAGONAL DIREITA - fita R
- TRIG_DIR = 43
- ECHO_DIR = 42

### Alimentação

- A **USB do Arduino** não é suficiente para alimentar os motores
- Use uma **bateria externa** (7.4V ~ 12V) para os drivers BTS7960
- Conecte o **GND** da bateria ao GND do Arduino
