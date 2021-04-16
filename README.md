# robosimples
Sistema para movimentação do Robô do Túnel de Movimento usando OEM750


A placa do robô do túnel de vento queimou. O sistema usa indexadores OEM-750 para controlar os motores de passo. Esta é uma tentativa simples de controlar os motores de passo. A idéia é, mais para frente fazer um sistema completo que consiga gerar movimentos completos com sensores de fim de curso, etc.

Por enquanto isso aqui é simplesmente um "laboratório" para explorar o problema. Este "laboratório tem as seguintes componentes:

 - Placa eletrônica para 3 eixos controlados por ESP32 Devkit v1 com 30 pinos. Projeto KICAD está na pasta robosimples
 - Programa arduino e C++ que vai na placa localizado na pasta roboesp32
 - Programa + servidor XML-RPC em python
 - Interfaces para outras linguagens de programação.

