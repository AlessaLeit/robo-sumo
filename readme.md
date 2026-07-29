\# 🤖 Robô Sumô Autônomo — Arduino



Código-fonte de um robô sumô autônomo baseado em Arduino, utilizando tração diferencial (2 rodas), sensores ultrassônicos HC-SR04 e drivers BTS7960.



\## 📋 Visão Geral



O robô executa 3 fases principais:



1\. \*\*Inicialização e busca da borda\*\* — Avança até detectar a borda branca do dojo

2\. \*\*Manobra\*\* — Recua e gira 180° para ficar de costas para a borda

3\. \*\*Busca e ataque\*\* — Varre o ambiente com ultrassons e ataca o inimigo



\## 🧰 Componentes



Componente - Quantidade 

* Arduino Mega 2560 - 1 
* Driver BTS7960 (Ponte H) - 2 
* Motor DC - 2 
* Sensor Ultrassônico HC-SR04 - 2 
* Sensor de Linha TCRT5000  - 1 
* Bateria externa (para motores) - 1



\## 🔌 Mapeamento de Pinos



\### Drivers BTS7960



\*\* DRIVER 1 — Motor ESQUERDO (Verde/Laranja) \*\*

D1\_RPWM      53

D1\_LPWM      50

D1\_RENABLE   52

D1\_LENABLE   51



\*\* DRIVER 2 — Motor DIREITO (Amarelo/Roxo) \*\*

D2\_RPWM      23

D2\_LPWM      24

D2\_RENABLE   22

D2\_LENABLE   25



\### Alimentação



\- A \*\*USB do Arduino\*\* não é suficiente para alimentar os motores

\- Use uma \*\*bateria externa\*\* (7.4V \~ 12V) para os drivers BTS7960

\- Conecte o \*\*GND\*\* da bateria ao GND do Arduino



\## 🧪 Como Testar



\### 1. Teste dos Motores



Envie `codigo\_robosumo.ino` para o Arduino. O robô executará em sequência:



1\. Ambos para frente (3s)

2\. Parada (2s)

3\. Ambos para ré (3s)

4\. Parada (2s)

5\. Giro horário (3s)

6\. Parada (2s)

7\. Giro anti-horário (3s)

8\. Parada (5s) → repete





