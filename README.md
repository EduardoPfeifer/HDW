# HDW
v1
Módulo medidor de consumo de energia elétrica (Bateria)
Versão: 01.00
Data: 08/08/16

1 Objetivo
Desenvolver um protótipo funcional de um Módulo medidor de energia elétrica dual
(invasivo e não invasivo). Ele deverá ser instalado dentro de caixa de passagem padrão NBR
5410 (4”x2” embutida) para medir o consumo de energia de um ponto qualquer da instalação
elétrica predial.

2 Descrição
O protótipo proposto neste documento consiste de um módulo, baseado em
microcontrolador, capaz de atuar como medidor sobre um ponto de consumo de energia. Este
módulo deverá efetuar medida não invasiva e/ou invasiva. A solução não invasiva deverá ser baseada no uso
de bobina de acoplamento indireto (bobina de Rogowski) e a invasiva pelo modulo Sensor de Corrente ACS712. 
O módulo poderá ser controlado remotamente através de uma interface de rádio operando na banda de 433M Hz e as
informações de consumo serão enviadas também pela interface de rádio atéum Getway. Além da capacidade de
medir consumo de energia, o módulo deverá apresentar uma saída ON/OFF isolada para atuar
sobre o consumo do ponto de energia. 

3 Arquitetura de Software e Hardware
3.1 Arquitetura de Software
A arquitetura de software deverá ser composta por três componentes principais 
- Interpretador de Comandos;
- Controlador;
- Supervisor de Consumo.

